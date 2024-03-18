using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Saper
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Start_Button_Click(object sender, RoutedEventArgs e)
        {
            int bombs = new int();
            int mapSize = new int();

            if (!int.TryParse(bombsTextBox.Text, out bombs) || !int.TryParse(sizeTextBox.Text, out mapSize) )
            {
                MessageBox.Show("W pola wpisano zły format danych!");
                return;
            }

            if (bombs >= mapSize * mapSize - 1)
            {
                MessageBox.Show("Wybrano za dużą liczbę bomb na podane wymiary mapy!");
                return;
            }
            if ( mapSize < 5)
            {
                MessageBox.Show("Za małe wymiary mapy! (min. 5x5)");
                return;
            }
            if( bombs < 1 )
            {
                MessageBox.Show($"Za mała ilość bomb (min. 1)");
                return;
            }
            new GameWindow(mapSize, bombs).ShowDialog();
        }

        private void tb_size_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox? tb = sender as TextBox;
            if (tb != null)
            {
                string? s = tb.Text;
                if ( int.TryParse(s, out int a) )
                {
                    l_map.Content = $"{a}x{a}";
                    l_rcmd.Content = $"Recommended: {(a*a / 4)}";
                }
                else
                {
                    l_map.Content = "0x0";
                    l_rcmd.Content = "Recommended: ?";
                }
            }
        }
    }
}
