using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace ProgramKalkulacyjnyGUI
{
    public class Month : Tab
    {
        //Zmienne
        private readonly TextBlock tb;

        //Konstruktory
        public Month(int index) : base(index)
        {
            //bloczek uzupelniajacy
            var rowDef = new RowDefinition
            {
                Height = new GridLength(15.0, GridUnitType.Pixel) //wysokosc przycisku (15.0px)
            };
            Gui.NameGrid.RowDefinitions.Add(rowDef);

            tb = new TextBlock
            {
                Background = Brushes.White
            };
            Grid.SetRow(tb, 1);
            Gui.NameGrid.Children.Add(tb);
        }

        //Metody
        public override void AddSection()
        {
            Grid.SetRow(tb, Grid.GetRow(tb) + 1);
            base.AddSection();
        }
    }
}
