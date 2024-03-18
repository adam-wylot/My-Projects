using System.Collections.Generic;
using System.Globalization;
using System.Linq;

namespace ProgramKalkulacyjnyGUI
{
    internal class OverrallSummary : Tab
    {
        //Zmienne
        private static List<List<Worker>>? workers;
        private static List<string> notes = new List<string>();
        private List<Worker> sectionWorkers;

        private readonly Section sec;
        private readonly SectionGUI secGUI;
        private readonly List<Row> rows;

        public static List<List<Worker>>? Workers { get => workers; set => workers = value; }
        public static List<string> Notes { get => notes; set => notes = value; }

        //Konstruktory
        public OverrallSummary(int index) : base(index)
        {
            sectionWorkers = new List<Worker>();

            AddSection();
            sec = Sections[0];
            secGUI = Sections[0].Gui;
            rows = sec.Rows;
            ClearSection();
            SetSummary();
        }
        //Metody
        //Statyczne
        public static void ConstructOverrrallSummary()
        {
            workers = new List<List<Worker>>();
            for (int i = 0; i < 12; i++)
            {
                workers.Add(new List<Worker>());
            }
        }
        public static void AddWorker(Worker worker)
        {
            if (worker.Month != null && workers != null)
            {
                int monthNum = worker.Month.Index;
                workers[monthNum].Add(worker);
            }
        }
        public static void RemoveWorker(Worker worker)
        {
            if (worker.Month != null && workers != null)
            {
                int monthNum = worker.Month.Index;
                workers[monthNum].Remove(worker);
            }
        }

        private void ClearSection()
        {
            secGUI.NameGrid.Children.Remove(secGUI.DeleteSectionBt);
            secGUI.NameGrid.Children.Remove(secGUI.AddBt);
            secGUI.NameGrid.Children.Remove(secGUI.LastRowNG);
            secGUI.Grid.Children.Remove(secGUI.PenultimentRow);
            secGUI.Grid.Children.Remove(secGUI.LastRow);
            for (int i = 0; i < 2; i++)
            {
                secGUI.NameGrid.RowDefinitions.RemoveAt(secGUI.NameGrid.RowDefinitions.Count - 1);
                secGUI.Grid.RowDefinitions.RemoveAt(secGUI.Grid.RowDefinitions.Count - 1);
            }
            ProgramRepository.ChangeCalculationSpaceSize(secGUI.SectionRowDef, -2 * ProgramRepository.StandardRowSize);
            rows[0].Gui.RemoveThisRow();
        }
        private void AddRow(Worker worker)
        {
            rows.Add(new Row(index: sec.Lp, rowHeight: ProgramRepository.StandardRowSize, nameGrid: secGUI.NameGrid, grid: secGUI.Grid, section: sec));
            secGUI.NameGrid.Children.Remove(rows[^1].Gui.DeleteRowButton);
            sec.Lp++;
            ProgramRepository.ChangeCalculationSpaceSize(secGUI.SectionRowDef, ProgramRepository.StandardRowSize);

            //blokowanie textboxow
            rows[^1].Gui.NameTb.IsReadOnly = true;
            rows[^1].Gui.NameTb.TextChanged -= rows[^1].Gui.Name_Changed;
            foreach (var textbox in rows[^1].Gui.TextBoxes)
            {
                textbox.IsReadOnly = true;
                textbox.TextChanged -= rows[^1].Gui.TextChangedEventHandler;
            }

            rows[^1].Worker.Name = worker.Name;
            rows[^1].Worker.Values = (double[])worker.Values.Clone();
        }
        private void RemoveRow(int rowIndex)
        {
            Row row = rows[rowIndex];

            sec.Lp--;
            sec.TabBelonging.Summary.ChangeLP(-1);
            ProgramRepository.ChangeCalculationSpaceSize(secGUI.SectionRowDef, -secGUI.Grid.RowDefinitions[0].Height.Value);

            //Usuwanie obiektów z wiersza
            Gui.NameGrid.Children.Remove(row.Gui.NumberTb);
            Gui.NameGrid.Children.Remove(row.Gui.NameTb);
            for (int i = 0; i < row.Gui.TextBoxes.Length; i++)
            {
                Gui.Grid.Children.Remove(row.Gui.TextBoxes[i]);
            }

            Gui.NameGrid.RowDefinitions.Remove(row.Gui.NameGridRowDef);
            Gui.Grid.RowDefinitions.Remove(row.Gui.RowDef);
            rows.Remove(row);

            //Przesuwanie pozostałych obiektów
            for (int i = rowIndex; i < rows.Count; i++)
            {
                rows[i].Gui.ReducePositionArr();
            }
        }
        public void SetSummary()
        {
            for (int i = 0; i < notes.Count; i++)
            {
                secGUI.FirstRow[i].Text = notes[i];
            }

            if (workers != null && sectionWorkers != null)
            {
                for (int i = 0; i < workers.Count; i++)
                {
                    foreach (Worker worker in workers[i])
                    {
                        if (worker.Name != null)
                        {
                            Worker? w = sectionWorkers.FirstOrDefault(x => x.Name == worker.Name);
                            if (w == null) //dodanie elementu do listy
                            {
                                sectionWorkers.Add(worker);
                                AddRow(worker);
                            }
                            else //dodanie wartości
                            {
                                int index = sectionWorkers.IndexOf(w);
                                rows[index].Worker.Name = worker.Name;
                                for (int j = 0; j < rows[index].Worker.Values.Length; j++)
                                {
                                    rows[index].Worker.Values[j] += worker.Values[j];
                                }
                            }
                        }
                    }
                }
                UpdateTextBoxes();
            }
        }
        private void UpdateTextBoxes()
        {
            for (int i = 0; i < rows.Count; i++)
            {
                rows[i].Gui.NameTb.Text = rows[i].Worker.Name;
                for (int j = 0; j < rows[i].Gui.TextBoxes.Length; j++)
                {
                    rows[i].Gui.TextBoxes[j].Text = rows[i].Worker.Values[j].ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
                }
            }
            sec.TabBelonging.Summary.UpdateSummary();
        }
    }
}
