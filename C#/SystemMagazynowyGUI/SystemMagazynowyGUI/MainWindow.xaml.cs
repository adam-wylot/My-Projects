using System;
using System.Collections.Generic;
using System.Linq;
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
using SystemMagazynowyGUI.Okna;

namespace SystemMagazynowyGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private UserRepository userRepository;

        public MainWindow()
        {
            InitializeComponent();
            userRepository = new UserRepository(new DataContext());
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button bt)
            {
                string? tag = bt.Tag.ToString();
                switch(tag)
                {
                    case "dodaj":
                        new DodajTowarWindow(userRepository).ShowDialog();
                        break;
                    case "edytuj":
                        new ListaWindow(userRepository, 0).ShowDialog();
                        break;
                    case "usun":
                        new ListaWindow(userRepository, 1).ShowDialog();
                        break;
                    case "towary":
                        new ListaWindow(userRepository, 2).ShowDialog();
                        break;
                    case "zamowienia":
                        new ListaWindow(userRepository, 3).ShowDialog();
                        break;
                    default:
                        MessageBox.Show("Wystąpił Błąd!");
                        break;
                }
            }
            else
            {
                MessageBox.Show("Wystąpił Błąd!");
            }
        }
    }
}


//dotnet ef dbcontext scaffold "server = localhost; database = SystemMagazynowy2; trusted_connection = true;"  Microsoft.EntityFrameworkCore.SqlServer