using System.Collections.Generic;

namespace ProgramKalkulacyjnyGUI
{
    public abstract class Tab
    {
        //Zmienne
        private TabGUI gui;

        private int index;
        private List<Section> sections;
        private Summary summary;

        public int Index { get => index; set => index = value; }
        public List<Section> Sections { get => sections; set => sections = value; }
        public Summary Summary { get => summary; set => summary = value; }
        internal TabGUI Gui { get => gui; }


        //Konstruktory
        public Tab(int index)
        {
            this.index = index;

            gui = new TabGUI(this);

            sections = new List<Section>();
            summary = new Summary(this);
        }

        //Metody
        public virtual void AddSection()
        {
            gui.AddSection();
            sections.Add(new Section(this));
        }
        internal void DeleteSection(SectionGUI secGUI, Section sec)
        {
            gui.DeleteSection(secGUI, sec);
        }
        public void ThisTabSelected()
        {
            double size = 0.0;
            foreach (var section in sections)
            {
                foreach (var rowDef in section.Gui.Grid.RowDefinitions)
                {
                    size += rowDef.Height.Value;
                }
            }
            ProgramRepository.ChangeCalculationSpaceSize(null, size);
        }
    }
}
