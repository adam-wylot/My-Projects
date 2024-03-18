using System.Collections.Generic;

//28 kolumn

namespace ProgramKalkulacyjnyGUI
{
    public class Section
    {
        //Zmienne
        private SectionGUI gui;

        private readonly Tab tabBelonging;
        private int index;
        private int lp;
        private List<Row> rows;

        internal Tab TabBelonging => tabBelonging;
        public int Index { get => index; set => index = value; }
        public int Lp { get => lp; set => lp = value; }
        public List<Row> Rows { get => rows; set => rows = value; }
        internal SectionGUI Gui { get => gui; }

        //Konstruktory
        public Section(Tab tabBelonging)
        {
            this.tabBelonging = tabBelonging;
            lp = 0;
            index = tabBelonging.Sections.Count;
            rows = new List<Row>();

            gui = new SectionGUI(this);
        }

        //Metody
        public void AddRow()
        {
            gui.AddRow();
        }
    }
}
