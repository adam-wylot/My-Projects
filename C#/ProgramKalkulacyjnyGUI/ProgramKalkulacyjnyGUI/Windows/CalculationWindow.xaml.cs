using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Media;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ProgramKalkulacyjnyGUI
{
    /// <summary>
    /// Logika interakcji dla klasy CalculationWindow.xaml
    /// </summary>
    public partial class CalculationWindow : Window
    {
        //Zmienne
        private readonly ProgramRepository programRepository;
        Grid definitionGrid;
        TextBox toPayTb;

        //Konstruktory
        public CalculationWindow(string filePath, FileTypes fileType) //Nowy plik
        {
            InitializeComponent();

            programRepository = new ProgramRepository(filePath, fileType, freeSpaceRD, this);
            ConstructDefinitionGrid(fileType);
            toPayTb.Text = $"Do wypłaty (31.{monthsTc.SelectedIndex + 1:00}.{programRepository.CreationTime.Year})"; // :00 to inaczej: .ToString("00")

            OverrallSummary.ConstructOverrrallSummary();

            var save = new Save();
            save.Serialize(filePath);
            SetImages();
        }
        public CalculationWindow(string filePath) //Wczytywanie pliku
        {
            InitializeComponent();

            Save save;
            try
            {
                save = Save.LoadXML(filePath);
            }
            catch (DirectoryNotFoundException ex)
            {
                SystemSounds.Hand.Play();
                if (ex.Message == "")
                {
                    MessageBox.Show("Nie podano adresu pliku!");
                }
                else
                {
                    MessageBox.Show("Nie odnaleziono pliku pod adresem: " + ex.Message);
                }
                Close();
                throw;
            }
            catch (InvalidOperationException)
            {
                SystemSounds.Hand.Play();
                MessageBox.Show("Wystąpił błąd podczas wczytywania pliku! Plik jest prawdopodobnie naruszony.");
                Close();
                throw;
            }
            catch (Exception ex)
            {
                SystemSounds.Hand.Play();
                MessageBox.Show("Wystąpił nieznany błąd podczas wczytywania pliku!" + ex.Message);
                Close();
                throw;
            }

            programRepository = new ProgramRepository(filePath, save.FileType, freeSpaceRD, this);
            programRepository.CreationTime = save.CreationTime;
            mainTb.Document.Blocks.Clear();
            mainTb.Document.Blocks.Add(new Paragraph(new Run(save.MainTb)));

            ConstructDefinitionGrid(programRepository.FileType);
            toPayTb.Text = $"Do wypłaty (31.{monthsTc.SelectedIndex + 1:00}.{programRepository.CreationTime.Year})"; // :00 to inaczej: .ToString("00")
            OverrallSummary.ConstructOverrrallSummary();

            try
            {
                LoadSections(save);
                LoadWorkers(save);
                OverrallSummary.Notes = new List<string>(save.Notes[^1][0]);
            }
            catch (Exception ex)
            {
                SystemSounds.Hand.Play();
                MessageBox.Show($"Wystąpił błąd podczas czytania pliku! ({ex})");
                Close();
                throw;
            }

            SetImages();
        }

        //Metody
        private void SetImages()
        {
            try //Zapis
            {
                saveBtImage.Source = new BitmapImage(new Uri(AppDomain.CurrentDomain.BaseDirectory + @"\Images\saveIcon.png"));
                saveBt.Background = new SolidColorBrush(Color.FromRgb(241, 198, 145));
            }
            catch (Exception)
            {
                saveBt.Content = "Zapisz";
                saveBt.FontSize = 16;
                saveBt.FontWeight = FontWeights.Bold;
            }

            try //Drukowanie
            {
                printBtImage.Source = new BitmapImage(new Uri(AppDomain.CurrentDomain.BaseDirectory + @"\Images\printIcon.png"));
                printBt.Background = new SolidColorBrush(Color.FromRgb(241, 198, 145));
            }
            catch (Exception)
            {
                printBt.Content = "Drukuj";
                printBt.FontSize = 16;
                printBt.FontWeight = FontWeights.Bold;
            }
        }
        private void LoadSections(Save save)
        {
            for (int i = 0; i < save.Notes.Count; i++) //miesiace
            {
                for (int j = 0; j < save.Notes[i].Count; j++) //sekcje
                {
                    programRepository.Months[i].AddSection();
                    programRepository.Months[i].Sections[^1].Rows[^1].Gui.RemoveThisRow();
                    for (int k = 0; k < save.Notes[i][j].Count; k++) //textboxy
                    {
                        if (programRepository.Months[i].Sections[j].Gui.FirstRow[k] is TextBox tb)
                        {
                            tb.Text = save.Notes[i][j][k];
                        }
                    }
                }
            }
        }
        private void LoadWorkers(Save save)
        {
            for (int i = 0; i < save.Workers.Count; i++)
            {
                foreach (Worker worker in save.Workers[i])
                {
                    programRepository.Months[i].Sections[worker.SectionIndex].AddRow();
                    if (worker.Name != null)
                    {
                        programRepository.Months[i].Sections[worker.SectionIndex].Rows[^1].ChangeName(worker.Name);
                        programRepository.Months[i].Sections[worker.SectionIndex].Rows[^1].Gui.NameTb.Text = worker.Name;
                    }
                    for (int j = 0; j < worker.Values.Length; j++)
                    {
                        programRepository.Months[i].Sections[worker.SectionIndex].Rows[^1].ChangeValue(j, worker.Values[j]);
                        if (worker.Values[j] != 0)
                        {
                            programRepository.Months[i].Sections[worker.SectionIndex].Rows[^1].Gui.TextBoxes[j].Text = worker.Values[j].ToString();
                        }
                    }
                }

                if (i < programRepository.Months.Length)
                {
                    programRepository.Months[i].Summary.UpdateSummary();
                }
            }
        }
        private bool SaveFile(string path)
        {
            var save = new Save();
            if (save.Serialize(path))
            {
                SystemSounds.Hand.Play();
                MessageBox.Show("Pomyślnie zapisano plik");
                return true;
            }
            else
            {
                SystemSounds.Hand.Play();
                MessageBox.Show("Wystąpił błąd podczas zapisywania pliku!");
                return false;
            }
        }
        private void ConstructDefinitionGrid(FileTypes fileType)
        {
            //Grid: definitionGrid, ScrollViewer: descrSCV
            var items = (new Grid(), new TextBox());
            switch (fileType)
            {
                case FileTypes.kierowcy:
                    items = programRepository.ConstructForKierowcy();
                    break;
                case FileTypes.montaz:

                    break;
                case FileTypes.montaz_zl:

                    break;
            }

            definitionGrid = items.Item1;
            toPayTb = items.Item2;

            descrSCV.Content = definitionGrid;
        }
        //Przyciski
        private void New_Section_Button_Click(object sender, RoutedEventArgs e)
        {
            if (programRepository.ActualMonth != null)
            {
                programRepository.ActualMonth.AddSection();
            }
        }
        private void Save_Button_Click(object sender, RoutedEventArgs e)
        {
            SaveFile(programRepository.FilePath);
        }
        private void SaveAs_Button_Click(object sender, RoutedEventArgs e)
        {
            var fileDialog = new SaveFileDialog();
            string[] strArr = ProgramRepository.Repository.FilePath.Split('\\');
            fileDialog.FileName = strArr[^1];
            fileDialog.InitialDirectory = ProgramRepository.Repository.FilePath;
            fileDialog.AddExtension = true;
            fileDialog.OverwritePrompt = true;
            fileDialog.CheckPathExists = true;
            fileDialog.Filter = "Plik kalkulacyjny|*.xalc";
            fileDialog.Title = "Zapisz jako...";
            fileDialog.DefaultExt = ".xalc";
            if (fileDialog.ShowDialog() == true)
            {
                if (SaveFile(fileDialog.FileName) )
                {
                    ProgramRepository.Repository.FilePath = fileDialog.FileName;
                }
            }
        }
        private void printBt_Click(object sender, RoutedEventArgs e)
        {
            PrintDialog printDialog = new PrintDialog();
            if (printDialog.ShowDialog() == true)
            {
                printDialog.PrintVisual(calculationGrid, "My First Print Job");
            }
        }
        //Zmieniono wartosc obiektu
        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            descrSCV.UpdateLayout();
            scrollBar.Maximum = descrSCV.ScrollableWidth;
            scrollBar.ViewportSize = descrSCV.ScrollableWidth;
        }
        private void ScrollBar_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            scrollBar.UpdateLayout();
            calculationSCV.ScrollToHorizontalOffset(scrollBar.Value);
            descrSCV.ScrollToHorizontalOffset(scrollBar.Value);

        }
        private void ScrollViewer_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            if (sender is ScrollViewer scV)
            {
                scV.UpdateLayout();
                nameSCV.ScrollToVerticalOffset(scV.VerticalOffset);
            }
        }
        private void NameSCV_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            nameSCV.UpdateLayout();
            calculationSCV.ScrollToVerticalOffset(nameSCV.VerticalOffset);
        }
        private void MonthsTc_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (programRepository.ActualMonth != null)
            {
                calculationGrid.Children.Remove(programRepository.ActualMonth.Gui.Grid);
                nameGrid.Children.Clear();
            }

            freeSpaceRD.Height = new GridLength(25.0, GridUnitType.Pixel);

            if (monthsTc.SelectedIndex == 12)
            {
                newSectionButton.Visibility = Visibility.Hidden;
                toPayTb.Text = "Do wypłaty";
                programRepository.Months[12] = new OverrallSummary(12);
            }
            else
            {
                newSectionButton.Visibility = Visibility.Visible;
                toPayTb.Text = $"Do wypłaty (31.{monthsTc.SelectedIndex + 1:00}.{programRepository.CreationTime.Year})"; // :00 to inaczej: .ToString("00")
            }

            programRepository.ActualMonth = programRepository.Months[monthsTc.SelectedIndex];
            programRepository.ActualMonth.ThisTabSelected();
            calculationGrid.Children.Add(programRepository.ActualMonth.Gui.Grid);
            nameGrid.Children.Add(programRepository.ActualMonth.Gui.NameGrid);
        }
    }
}
