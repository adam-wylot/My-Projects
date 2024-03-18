using System.Windows;
using System.Windows.Controls;

namespace ProgramKalkulacyjnyGUI
{
    internal class TabGUI
    {
        private readonly Tab dataTab;

        private readonly Grid nameGrid;
        private readonly Grid grid;

        public Grid NameGrid { get => nameGrid; }
        public Grid Grid { get => grid; }

        //Konstruktory
        public TabGUI(Tab dataTab)
        {
            this.dataTab = dataTab;

            nameGrid = new Grid();
            grid = new Grid();
        }

        //Metody
        public void AddSection()
        {
            var rowDef = new RowDefinition
            {
                Height = new GridLength(0, GridUnitType.Auto)
            };
            nameGrid.RowDefinitions.Insert(dataTab.Sections.Count, rowDef);

            rowDef = new RowDefinition();
            ProgramRepository.ChangeCalculationSpaceSize(rowDef, 4 * ProgramRepository.StandardRowSize);
            rowDef.Height = new GridLength(rowDef.Height.Value - 1.0, GridUnitType.Pixel); // -1.0 to poprawka bledu wielkosci piksela nieznajomego pochodzenia
            grid.RowDefinitions.Insert(grid.RowDefinitions.Count - 1, rowDef);

            Grid.SetRow(dataTab.Summary.Gui.NameGrid, Grid.GetRow(dataTab.Summary.Gui.NameGrid) + 1);
            Grid.SetRow(dataTab.Summary.Gui.Grid, Grid.GetRow(dataTab.Summary.Gui.Grid) + 1);
        }
        internal void DeleteSection(SectionGUI gui, Section sec)
        {
            nameGrid.Children.Remove(gui.NameGrid);
            grid.Children.Remove(gui.Grid);
            ProgramRepository.ChangeCalculationSpaceSize(gui.SectionRowDef, -1.0 * ProgramRepository.StandardRowSize * (sec.Lp + 3));
            nameGrid.RowDefinitions.RemoveAt(sec.Index);
            grid.RowDefinitions.Remove(gui.SectionRowDef);

            Grid.SetRow(dataTab.Summary.Gui.NameGrid, Grid.GetRow(dataTab.Summary.Gui.NameGrid) - 1);
            Grid.SetRow(dataTab.Summary.Gui.Grid, Grid.GetRow(dataTab.Summary.Gui.Grid) - 1);

            dataTab.Summary.ChangeLP(-sec.Rows.Count);
            dataTab.Sections.Remove(sec);
            dataTab.Summary.UpdateSummary();
        }
    }
}
