using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Input;

namespace ProgramKalkulacyjnyGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //Zmienne
        private List<RecentFileProperties> recentFiles;

        //Konstruktory
        public MainWindow()
        {
            InitializeComponent();

            recentFiles = Save.LoadRecentFiles();

            //sprawdzanie czy program nie jest otwierany za pomoca pliku z rozszerzeniem .xalc
            string[] s = Environment.GetCommandLineArgs();
            if (s.Length >= 2)
            {
                loadTb.Text = s[1];
                LoadFile();
            }

            recentProjectsListBox.ItemsSource = recentFiles;
        }

        //Przyciski
        private void New_Button_Click(object sender, RoutedEventArgs e)
        {
            new NewFileWindow(recentFiles).Show();
            Close();
        }
        private void Load_Button_Click(object sender, RoutedEventArgs e)
        {
            LoadFile();
        }
        private void LoadFile()
        {
            var selectedFile = recentFiles.FirstOrDefault(x => x.Path + x.Name == loadTb.Text);
            if (selectedFile != null)
            {
                recentFiles.Remove(selectedFile);
                recentFiles.Insert(0, selectedFile);
                Save.SerializeRecentFiles(recentFiles);
            }

            bool closeWin = true;
            try
            {
                var calcWin = new CalculationWindow(filePath: loadTb.Text);
                calcWin.Show();
            }
            catch (Exception)
            {
                closeWin = false;
            }

            if (closeWin)
            {
                Close();
            }
        }
        private void Search_For_Path_Button_Click(object sender, RoutedEventArgs e)
        {
            var fileDialog = new OpenFileDialog();
            fileDialog.Filter = "Plik kalkulacyjny|*.xalc"; //|Wszystkie pliki|*.*
            fileDialog.DefaultExt = ".xalc";
            fileDialog.Title = "Szukaj pliku";
            fileDialog.Multiselect = false;
            if (fileDialog.ShowDialog() == true)
            {
                loadTb.Text = fileDialog.FileName;
            }
        }

        private void recentProjectsListBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (recentProjectsListBox.SelectedItem is RecentFileProperties selectedFile)
            {
                recentFiles.Remove(selectedFile);
                recentFiles.Insert(0, selectedFile);
                Save.SerializeRecentFiles(recentFiles);

                bool closeWin = true;
                try
                {
                    new CalculationWindow(filePath: $"{selectedFile.Path}{selectedFile.Name}").Show();
                }
                catch (DirectoryNotFoundException)
                {
                    closeWin = false;
                    recentFiles.Remove(selectedFile);
                    Save.SerializeRecentFiles(recentFiles);
                    recentProjectsListBox.UpdateLayout();
                }
                catch (Exception)
                {
                    closeWin = false;
                }

                if (closeWin)
                {
                    Close();
                }
            }
        }
    }
}