using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace ProgramKalkulacyjnyGUI
{
    internal class SummaryGUI
    {
        private readonly Summary data;

        private readonly RowDefinition nameRowDef;
        private readonly RowDefinition calcRowDef;
        private readonly Grid nameGrid;
        private readonly Grid grid;
        private readonly TextBox workersNumTb;
        private readonly TextBox legendTb;
        private readonly TextBox[] summaryTbs;

        public Grid NameGrid { get => nameGrid; }
        public Grid Grid { get => grid; }
        public TextBox WorkersNumTb { get => workersNumTb; }
        public TextBox[] SummaryTbs { get => summaryTbs; }

        public SummaryGUI(Summary data)
        {
            this.data = data;

            nameRowDef = new RowDefinition
            {
                Height = new GridLength(0, GridUnitType.Auto)
            };
            data.Tab.Gui.NameGrid.RowDefinitions.Add(nameRowDef);

            calcRowDef = new RowDefinition();
            ProgramRepository.ChangeCalculationSpaceSize(calcRowDef, ProgramRepository.StandardRowSize);
            data.Tab.Gui.Grid.RowDefinitions.Add(calcRowDef);

            workersNumTb = new TextBox();
            legendTb = new TextBox();
            summaryTbs = new TextBox[ProgramRepository.Repository.ColumnsAmount];

            nameGrid = new Grid();
            Grid.SetRow(nameGrid, 0);
            data.Tab.Gui.NameGrid.Children.Add(nameGrid);

            grid = new Grid();
            Grid.SetRow(grid, 0);
            data.Tab.Gui.Grid.Children.Add(grid);

            Division();
            ProgramRepository.ChangeSizeOfColumns(nameGrid, ProgramRepository.Repository.NameColumnSizes);
            ProgramRepository.ChangeSizeOfColumns(grid, ProgramRepository.Repository.ColumnSizes);
            SetSummary();
        }

        //Metody
        private void Division()
        {
            for (int i = 0; i < 2; i++)
            {
                nameGrid.ColumnDefinitions.Add(new ColumnDefinition());
            }

            for (int i = 0; i < ProgramRepository.Repository.ColumnsAmount; i++)
            {
                grid.ColumnDefinitions.Add(new ColumnDefinition());
            }
        }
        private void SetSummary()
        {
            var rd = new RowDefinition
            {
                Height = new GridLength(ProgramRepository.StandardRowSize, GridUnitType.Pixel)
            };
            nameGrid.RowDefinitions.Add(rd);

            rd = new RowDefinition
            {
                Height = new GridLength(ProgramRepository.StandardRowSize, GridUnitType.Pixel)
            };
            grid.RowDefinitions.Add(rd);

            workersNumTb.Text = "=0";
            workersNumTb.TextAlignment = TextAlignment.Right;
            workersNumTb.VerticalContentAlignment = VerticalAlignment.Center;
            workersNumTb.IsReadOnly = true;
            workersNumTb.FontSize = 13;
            workersNumTb.Background = Brushes.LightGray;
            Grid.SetColumn(workersNumTb, 0);
            nameGrid.Children.Add(workersNumTb);

            legendTb.Text = "Razem:";
            legendTb.TextAlignment = TextAlignment.Right;
            legendTb.VerticalContentAlignment = VerticalAlignment.Center;
            legendTb.IsReadOnly = true;
            legendTb.FontSize = 16;
            legendTb.Background = Brushes.LightGray;
            legendTb.FontWeight = FontWeights.Bold;
            Grid.SetColumn(legendTb, 1);
            nameGrid.Children.Add(legendTb);

            for (int i = 0; i < ProgramRepository.Repository.ColumnsAmount; i++)
            {
                summaryTbs[i] = new TextBox
                {
                    Text = "=0",
                    TextAlignment = TextAlignment.Right,
                    VerticalContentAlignment = VerticalAlignment.Center,
                    IsReadOnly = true,
                    FontSize = 13,
                    Background = Brushes.LightGray,
                    FontWeight = FontWeights.Bold
                };
                Grid.SetColumn(summaryTbs[i], i);
                grid.Children.Add(summaryTbs[i]);
            }
        }
    }
}
