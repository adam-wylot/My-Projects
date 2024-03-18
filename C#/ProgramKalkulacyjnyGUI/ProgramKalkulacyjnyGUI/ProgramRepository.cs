using System;
using System.IO;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace ProgramKalkulacyjnyGUI
{
    public enum FileTypes
    {
        kierowcy,
        montaz,
        montaz_zl
    }

    public class ProgramRepository
    {
        //Stałe
        private static readonly double[] driversColumnSizes = { 75, 120, 90, 75, 110, 100, 75, 100, 100, 110, 100, 90, 90, 90, 100, 90, 120, 120, 120, 100, 100, 120, 120, 100 }; //24
        private static readonly string[] driversColumnNames = { "Brutto do kosztów", "Płaca Brutto ZUS", "Odprawa Pośmiertna", "Ekwiwalent za urlop", "Wynagrodzenie za godziny nadliczbowe", "Wynagrodzenie Urlopowe", "Premia uznaniowa", "Wynagrodzenie za czas choroby", "Ogółem Przychód", "", "9,76% Emerytalne", "1,5% Rentowe", "6,5% Rentowe", "2,45% Chorobowe", "Składki Razem", "", "Podstawa naliczenia składki zdrowotnej", "", "", "", "(9%) Składka Zdrowotna", "Do wypłaty ()", "Wypłata", "" };
        private static readonly double[] montageColumnSizes = { 75, 120, 90, 75, 110, 90, 75, 90, 100, 110, 100, 90, 90, 90, 100, 90, 120, 120, 120, 100, 100, 120, 120, 100 }; //24
        private static readonly double[] montageZLColumnSizes = { 75, 120, 90, 75, 110, 90, 75, 90, 100, 110, 100, 90, 90, 90, 100, 90, 120, 120, 120, 100, 100 }; //21 (Do poprawy)
        private static readonly double standardRowSize = 25.0;

        //Zmienne
        private static RowDefinition? freeSpaceRD;
        private static ProgramRepository? repository;

        private CalculationWindow calcWin;
        private FileTypes fileType;
        private string filePath;
        private DateTime creationTime = DateTime.Now;
        private Tab[] months;
        private Tab? actualMonth;
        private double[] nameColumnSizes = { 30, 160 };
        private double[] columnSizes = Array.Empty<double>();
        private int columnsAmount = 0;
        private int propertiesTbAmount = 0;

        public CalculationWindow CalcWin => calcWin;
        public static double StandardRowSize { get => standardRowSize; }
        public FileTypes FileType { get => fileType; set => fileType = value; }
        public string FilePath { get => filePath; set => filePath = value; }
        public DateTime CreationTime { get => creationTime; set => creationTime = value; }
        public Tab[] Months { get => months; set => months = value; }
        public Tab? ActualMonth { get => actualMonth; set => actualMonth = value; }
        public double[] NameColumnSizes { get => nameColumnSizes; set => nameColumnSizes = value; }
        public double[] ColumnSizes { get => columnSizes; set => columnSizes = value; }
        public int ColumnsAmount { get => columnsAmount; set => columnsAmount = value; }
        public int PropertiesTbAmount { get => propertiesTbAmount; set => propertiesTbAmount = value; }
        public static ProgramRepository? Repository { get => repository; }


        //Konstruktory
        public ProgramRepository(string filePath, FileTypes fileType, RowDefinition rowDef, CalculationWindow window)
        {
            calcWin = window;

            if (repository == null)
            {
                repository = this;
            }

            this.filePath = filePath;
            this.fileType = fileType;
            freeSpaceRD = rowDef;
            months = new Tab[13];

            switch (fileType)
            {
                case FileTypes.kierowcy:
                    propertiesTbAmount = 8;
                    columnSizes = (double[])driversColumnSizes.Clone();
                    break;
                case FileTypes.montaz:
                    propertiesTbAmount = 8;
                    columnSizes = (double[])montageColumnSizes.Clone();
                    break;
                case FileTypes.montaz_zl:
                    propertiesTbAmount = 7;
                    columnSizes = (double[])montageZLColumnSizes.Clone();
                    break;
            }
            columnsAmount = columnSizes.Length;

            CreateMonths();
            actualMonth = Months[0];
        }

        //Metody
        private void CreateMonths()
        {
            for (int i = 0; i < months.Length - 1; i++)
            {
                months[i] = new Month(i);
            }
            months[12] = new OverrallSummary(12);
        }
        public (Grid, TextBox) ConstructForKierowcy()
        {
            var grid = new Grid();
            grid.RowDefinitions.Add(new RowDefinition
            {
                Height = new GridLength(pixels: 40)
            });
            grid.RowDefinitions.Add(new RowDefinition());
            TextBox textBox;
            var toPayTb = new TextBox();
            for (int i = 0; i < ColumnSizes.Length; i++)
            {
                grid.ColumnDefinitions.Add(new ColumnDefinition());
                if (driversColumnNames[i] != "")
                {
                    textBox = new TextBox
                    {
                        Text = driversColumnNames[i],
                        TextAlignment = TextAlignment.Center,
                        TextWrapping = TextWrapping.Wrap,
                        Background = Brushes.LightGray,
                        FontWeight = FontWeights.Bold,
                        IsReadOnly = true
                    };
                    Grid.SetRow(textBox, 1);
                    Grid.SetColumn(textBox, i);
                    grid.Children.Add(textBox);

                    if (i == 21)
                    {
                        toPayTb = textBox;
                    }
                }
            }
            ChangeSizeOfColumns(grid, ColumnSizes);

            return (grid, toPayTb);
        }

        //Statyczne
        public static void ChangeCalculationSpaceSize(RowDefinition? rw, double size)
        {
            if (rw != null)
            { rw.Height = new GridLength(rw.Height.Value + size, GridUnitType.Pixel); }
            if (freeSpaceRD != null)
            { freeSpaceRD.Height = new GridLength(freeSpaceRD.Height.Value + size, GridUnitType.Pixel); }

        }
        public static void FunctionUnavaible()
        {
            MessageBox.Show("Funkcja tymczasowo niedostępna.");
        }
        public static void ChangeSizeOfColumns(Grid grid, double[] tab)
        {
            GridLength gL;

            for (int i = 0; i < grid.ColumnDefinitions.Count; i++)
            {
                gL = new GridLength(tab[i], GridUnitType.Pixel);
                grid.ColumnDefinitions[i].Width = gL;
            }
        }
        public static void ChangeSizeOfRows(Grid grid)
        {
            GridLength gL;

            for (int i = 0; i < grid.RowDefinitions.Count; i++)
            {
                gL = new GridLength(standardRowSize, GridUnitType.Pixel);
                grid.RowDefinitions[i].Height = gL;
            }
        }
        public async static void MakeFile(string path)
        {
            try
            {
                await Task.Run(() =>
                {
                    if (File.Exists(path))
                    {
                        File.Delete(path);
                    }
                    File.Create(path);
                });
            }
            catch (Exception)
            {
                MessageBox.Show("Nie udało się utworzyć pliku!");
                throw;
            }

        }
    }
}
