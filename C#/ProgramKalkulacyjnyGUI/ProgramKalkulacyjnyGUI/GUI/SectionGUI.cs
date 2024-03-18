using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace ProgramKalkulacyjnyGUI
{
    internal class SectionGUI
    {
        private readonly Section data;

        private readonly Grid nameGrid;
        private readonly Grid grid;
        private readonly RowDefinition sectionRowDef;

        //Budowa sekcji
        private readonly Button deleteSectionBt;
        private readonly Button addBt;
        private readonly TextBlock lastRowNG;

        private readonly TextBox[] firstRow;
        private readonly TextBlock penultimentRow;
        private readonly TextBlock lastRow;

        //gettery i settery
        public Grid NameGrid { get => nameGrid; }
        public Grid Grid { get => grid; }
        public RowDefinition SectionRowDef { get => sectionRowDef; }
        public Button DeleteSectionBt { get => deleteSectionBt; }
        public Button AddBt { get => addBt; }
        public TextBlock LastRowNG { get => lastRowNG; }
        public TextBox[] FirstRow { get => firstRow; }
        public TextBlock PenultimentRow { get => penultimentRow; }
        public TextBlock LastRow { get => lastRow; }

        //Konstruktor
        public SectionGUI(Section data)
        {
            this.data = data;

            nameGrid = new Grid();
            Grid.SetRow(nameGrid, data.TabBelonging.Sections.Count);
            data.TabBelonging.Gui.NameGrid.Children.Add(nameGrid);

            grid = new Grid();
            Grid.SetRow(grid, data.TabBelonging.Sections.Count);
            sectionRowDef = data.TabBelonging.Gui.Grid.RowDefinitions[^2];
            data.TabBelonging.Gui.Grid.Children.Add(grid);

            deleteSectionBt = new Button();
            addBt = new Button();
            lastRowNG = new TextBlock();

            firstRow = new TextBox[ProgramRepository.Repository.ColumnsAmount + 1];
            penultimentRow = new TextBlock();
            lastRow = new TextBlock();

            Division(rows: 3);
            ProgramRepository.ChangeSizeOfColumns(nameGrid, ProgramRepository.Repository.NameColumnSizes);
            ProgramRepository.ChangeSizeOfRows(nameGrid);
            ProgramRepository.ChangeSizeOfColumns(grid, ProgramRepository.Repository.ColumnSizes);
            ProgramRepository.ChangeSizeOfRows(grid);

            SectionSetter();
        }

        //Metody
        private void Division(int rows)
        {
            for (int i = 0; i < rows; i++)
            {
                nameGrid.RowDefinitions.Add(new RowDefinition());
            }
            for (int i = 0; i < 2; i++)
            {
                nameGrid.ColumnDefinitions.Add(new ColumnDefinition());
            }

            for (int i = 0; i < rows; i++)
            {
                grid.RowDefinitions.Add(new RowDefinition());
            }
            for (int i = 0; i < ProgramRepository.Repository.ColumnsAmount; i++)
            {
                grid.ColumnDefinitions.Add(new ColumnDefinition());
            }
        }
        private void SectionSetter()
        {
            //Pierwszy wiersz
            deleteSectionBt.Content = "X";
            deleteSectionBt.FontWeight = FontWeights.Bold;
            deleteSectionBt.Background = Brushes.IndianRed;
            deleteSectionBt.Click += Delete_Section_Button_Click;
            Grid.SetRow(deleteSectionBt, 0);
            Grid.SetColumn(deleteSectionBt, 0);
            nameGrid.Children.Add(deleteSectionBt);

            TextBox tempTextBox;
            tempTextBox = new TextBox
            {
                Foreground = Brushes.Red,
                FontWeight = FontWeights.Bold
            };
            firstRow[0] = tempTextBox;
            Grid.SetRow(tempTextBox, 0);
            Grid.SetColumn(tempTextBox, 1);
            nameGrid.Children.Add(tempTextBox);

            for (int i = 1; i <= grid.ColumnDefinitions.Count; i++)
            {
                tempTextBox = new TextBox
                {
                    Foreground = Brushes.Red,
                    FontWeight = FontWeights.Bold
                };
                firstRow[i] = tempTextBox;

                Grid.SetRow(tempTextBox, 0);
                Grid.SetColumn(tempTextBox, i - 1);
                grid.Children.Add(firstRow[i]);
            }

            //Drugi wiersz
            data.Rows.Add(new Row(index: data.Lp, rowHeight: ProgramRepository.StandardRowSize, nameGrid: nameGrid, grid: grid, section: data));
            data.Lp++;

            //Trzeci wiersz
            addBt.Click += Add_Row_Button_Click;
            addBt.Content = "+";
            addBt.FontSize = 16;
            addBt.Background = Brushes.White;
            addBt.FontWeight = FontWeights.Bold;
            Grid.SetRow(addBt, 2);
            Grid.SetColumn(addBt, 0);
            Grid.SetColumnSpan(addBt, 2);
            nameGrid.Children.Add(addBt);

            penultimentRow.Background = Brushes.White;
            penultimentRow.Text = " (Dodaj wiersz)";
            penultimentRow.FontSize = 10;
            penultimentRow.Foreground = Brushes.LightGray;
            Grid.SetRow(penultimentRow, 2);
            Grid.SetColumn(penultimentRow, 0);
            Grid.SetColumnSpan(penultimentRow, ProgramRepository.Repository.ColumnsAmount);
            grid.Children.Add(penultimentRow);

            //Czwarty wiersz
            lastRowNG.Background = new SolidColorBrush(Color.FromRgb(187, 187, 187));
            Grid.SetRow(lastRowNG, 3);
            Grid.SetColumn(lastRowNG, 0);
            Grid.SetColumnSpan(lastRowNG, nameGrid.ColumnDefinitions.Count);
            nameGrid.Children.Add(lastRowNG);

            lastRow.Background = new SolidColorBrush(Color.FromRgb(187, 187, 187));
            Grid.SetRow(lastRow, 3);
            Grid.SetColumn(lastRow, 0);
            Grid.SetColumnSpan(lastRow, grid.ColumnDefinitions.Count);
            grid.Children.Add(lastRow);
        }
        private void Add_Row_Button_Click(object sender, RoutedEventArgs e)
        {
            AddRow();
        }
        public void AddRow()
        {
            data.Rows.Add(new Row(index: data.Lp, rowHeight: ProgramRepository.StandardRowSize, nameGrid: nameGrid, grid: grid, section: data));
            data.Lp++;
            ProgramRepository.ChangeCalculationSpaceSize(sectionRowDef, ProgramRepository.StandardRowSize);

            //Przesuwanie dwóch ostatnich wierszy o jedno pole
            Grid.SetRow(addBt, Grid.GetRow(addBt) + 1);
            Grid.SetRow(penultimentRow, Grid.GetRow(penultimentRow) + 1);

            Grid.SetRow(lastRowNG, Grid.GetRow(lastRowNG) + 1);
            Grid.SetRow(lastRow, Grid.GetRow(lastRow) + 1);
        }
        private void Delete_Section_Button_Click(object sender, RoutedEventArgs e)
        {
            data.TabBelonging.DeleteSection(this, data);

            //Przesuwanie ponizszych o jeden do gory
            for (int i = data.Index + 1; i < data.TabBelonging.Sections.Count; i++)
            {
                Grid.SetRow(data.TabBelonging.Sections[i].Gui.NameGrid, data.TabBelonging.Sections[i].Index - 1);
                Grid.SetRow(data.TabBelonging.Sections[i].Gui.Grid, data.TabBelonging.Sections[i].Index - 1);
                data.TabBelonging.Sections[i].Index--;
            }

            //Usuwanie wszystkich pracownikow z podliczenia
            foreach (Row row in data.Rows)
            {
                OverrallSummary.RemoveWorker(row.Worker);
            }
        }
    }
}
