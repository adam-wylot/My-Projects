using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Xml.Serialization;

namespace ProgramKalkulacyjnyGUI
{
    public class Save : ISerialization
    {
        //Zmienne
        private FileTypes fileType;
        private DateTime creationTime;
        private string mainTb;
        private List<List<Worker>> workers;
        private List<List<List<string>>> notes;
        public FileTypes FileType { get => fileType; set => fileType = value; }
        public DateTime CreationTime { get => creationTime; set => creationTime = value; }
        public string MainTb { get => mainTb; set => mainTb = value; }
        public List<List<Worker>> Workers { get => workers; set => workers = value; }
        public List<List<List<string>>> Notes { get => notes; set => notes = value; }

        //Kosntruktory
        public Save()
        {
            notes = new List<List<List<string>>>();

            if (OverrallSummary.Workers != null)
            {
                workers = OverrallSummary.Workers;
            }
            else
            {
                workers = new List<List<Worker>>();
            }

            if (ProgramRepository.Repository != null)
            {
                fileType = ProgramRepository.Repository.FileType;
                creationTime = ProgramRepository.Repository.CreationTime;
                mainTb = new TextRange(ProgramRepository.Repository.CalcWin.mainTb.Document.ContentStart, ProgramRepository.Repository.CalcWin.mainTb.Document.ContentEnd).Text;

                for (int i = 0; i < ProgramRepository.Repository.Months.Length; i++) //dopiski do sekcji
                {
                    notes.Add(new List<List<string>>());
                    for (int j = 0; j < ProgramRepository.Repository.Months[i].Sections.Count; j++)
                    {
                        notes[i].Add(new List<string>());
                        for (int k = 0; k < ProgramRepository.Repository.Months[i].Sections[j].Gui.FirstRow.Length; k++)
                        {
                            TextBox tb = ProgramRepository.Repository.Months[i].Sections[j].Gui.FirstRow[k];
                            notes[i][j].Add(tb.Text);
                        }
                    }
                }
            }
            else
            {
                mainTb = "";
            }
        }

        //Metody
        public bool Serialize(string filePath)
        {
            if (File.Exists(filePath))
            {
                try
                {
                    File.Delete(filePath);
                }
                catch (Exception) //IOException
                {
                    return false;
                }
            }
            try
            {
                var xml = new XmlSerializer(GetType());
                var streamWriter = new StreamWriter(filePath);
                xml.Serialize(streamWriter, this);
                streamWriter.Close();
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }
        public static Save LoadXML(string filePath)
        {
            if (!File.Exists(filePath))
            {
                throw new DirectoryNotFoundException(filePath);
            }
            var xml = new XmlSerializer(typeof(Save));
            var streamReader = new StreamReader(filePath);
            var save = xml.Deserialize(streamReader);
            streamReader.Close();

            if (save is Save s)
            {
                return s;
            }
            else
            {
                throw new Exception();
            }

        }
        public static bool SerializeRecentFiles(List<RecentFileProperties> list)
        {
            try
            {
                var xml = new XmlSerializer(list.GetType());
                var streamWriter = new StreamWriter("RecentFiles.xml");
                xml.Serialize(streamWriter, list);
                streamWriter.Close();
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }
        public static List<RecentFileProperties> LoadRecentFiles()
        {
            try
            {
                var xml = new XmlSerializer(typeof(List<RecentFileProperties>));
                var streamReader = new StreamReader("RecentFiles.xml");
                var save = xml.Deserialize(streamReader);
                streamReader.Close();

                if (save is List<RecentFileProperties> s)
                {
                    return s;
                }
                else
                {
                    throw new Exception();
                }
            }
            catch (Exception)
            {
                return new List<RecentFileProperties>();
            }
        }
    }
}

