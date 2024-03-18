using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace ProgramKalkulacyjnyGUI
{
    /// <summary>
    /// Logika interakcji dla klasy NewFileWindow.xaml
    /// </summary>
    public partial class NewFileWindow : Window
    {
        private List<RecentFileProperties> recentFiles;

        public NewFileWindow(List<RecentFileProperties> recentFiles)
        {
            InitializeComponent();

            this.recentFiles = recentFiles;
            filePathLb.Content = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\";

            SetImages();
        }

        //Metody
        private void SetImages()
        {
            try
            {
                logoImage.Source = new BitmapImage(new Uri(AppDomain.CurrentDomain.BaseDirectory + @"\Images\logo.png"));
            }
            catch (Exception) { }
            try
            {
                dirBtImage.Source = new BitmapImage(new Uri(AppDomain.CurrentDomain.BaseDirectory + @"\Images\directory.png"));
            }
            catch (Exception)
            {
                dirBt.Content = "...";
                dirBt.FontSize = 16;
            }
        }
        private void Make_File_Button_Click(object sender, RoutedEventArgs e)
        {
            MakeFile();
        }
        private void MakeFile()
        {
            if (CheckOptions())
            {
                if (fileTypeCb.SelectedItem is FileTypes fileType)
                {
                    if (fileNameTb.Text.Length < 5 || fileNameTb.Text.Substring(fileNameTb.Text.Length - 5, 5) != ".xalc")
                    {
                        fileNameTb.Text = fileNameTb.Text + ".xalc";
                    }

                    //Obsluga listy ostatio uzywanych plikow
                    var existingFile = recentFiles.FirstOrDefault(x => x.Path == filePathLb.Content.ToString() && x.Name == fileNameTb.Text);
                    if (existingFile != null)
                    {
                        recentFiles.Remove(existingFile);
                    }

                    recentFiles.Insert(0, new RecentFileProperties(name: fileNameTb.Text, path: filePathLb.Content.ToString()));
                    if (recentFiles.Count > 10)
                    {
                        recentFiles.RemoveRange(10, recentFiles.Count - 10);
                    }
                    Save.SerializeRecentFiles(recentFiles);

                    string s = $"{filePathLb.Content}{fileNameTb.Text}";
                    bool tmp = true;
                    try
                    {
                        ProgramRepository.MakeFile(s);
                    }
                    catch (Exception)
                    {
                        tmp = false;
                    }

                    if (tmp)
                    {
                        new CalculationWindow(filePath: s, fileType).Show();
                        Close();
                    }
                }
            }
        }
        private bool CheckOptions()
        {
            if (fileNameTb.Text == "")
            {
                MessageBox.Show("Podaj nazwę pliku!");
                return false;
            }
            if (fileTypeCb.SelectedIndex == -1)
            {
                MessageBox.Show("Wybierz typ pliku!");
                return false;
            }

            return true;
        }

        private void Find_Path_Button_Click(object sender, RoutedEventArgs e)
        {
            var fileDialog = new SaveFileDialog();
            fileDialog.FileName = fileNameTb.Text;
            fileDialog.InitialDirectory = filePathLb.Content.ToString();
            fileDialog.AddExtension = true;
            fileDialog.OverwritePrompt = true;
            fileDialog.CheckPathExists = true;
            fileDialog.Filter = "Plik kalkulacyjny|*.xalc";
            fileDialog.Title = "Zapisz plik";
            fileDialog.DefaultExt = ".xalc";
            if (fileDialog.ShowDialog() == true)
            {
                int len = fileDialog.SafeFileName.Length;
                fileNameTb.Text = fileDialog.SafeFileName;
                filePathLb.Content = fileDialog.FileName.Substring(0, fileDialog.FileName.Length - len);
            }
        }
    }
}
