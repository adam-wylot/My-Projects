using Shared;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using System.Windows.Shapes;

namespace SystemMagazynowyGUI.Okna
{
    public partial class ListaWindow : Window
    {
        private UserRepository userRepository;
        private ObservableCollection<Towar> towary;

        public ListaWindow(UserRepository userRepository)
        {
            InitializeComponent();
            this.userRepository = userRepository;
            towary = new ObservableCollection<Towar>(userRepository.GetTowary());
            lista.ItemsSource = towary;
        }
        public ListaWindow(UserRepository userRepository, int typOkna) : this(userRepository)
        {
            switch(typOkna)
            {
                case 0: //edycja
                    lista.MouseDoubleClick += edycja_MouseDoubleClick;
                    break;
                case 1: //usuwanie
                    lista.MouseDoubleClick += usuwanie_MouseDoubleClick;
                    break;
                case 2: //przeglad towarow
                    string[] s = {"Nazwa", "Kod towaru"};
                    comboBox.ItemsSource = s;
                    comboBox.Visibility = Visibility.Visible;
                    label.Visibility = Visibility.Visible;
                    break;
                case 3: //przeglad zamowien
                    lista.ItemsSource = userRepository.GetZamowienia();
                    button.Visibility = Visibility.Visible;
                    break;
                default:
                    MessageBox.Show("Wystąpił Błąd!");
                    Close();
                    break;
            }
        }

        private void edycja_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lista.SelectedItem is Towar towar)
            {
                new DodajTowarWindow(userRepository, towar.Id).ShowDialog();
            }
        }
        private void usuwanie_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lista.SelectedItem is Towar towar)
            {
                if (userRepository.UsunTowar(towar))
                {
                    towary.Remove(towar);
                }
                else
                {
                    MessageBox.Show("Nie można usunąć towaru!");
                }
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (lista.SelectedItem is Zamowienie zamowienie)
            {
                zamowienie.DataRealizacji = DateTime.Now;

                var zamTow = userRepository.Context.ZamowieniaTowary.Where(x => x.Zamowienie == zamowienie);
                foreach (var item in zamTow)
                {
                    var towar = userRepository.Context.Towary.FirstOrDefault(x => x.Kod == item.Towar.Kod);
                    if (towar != null)
                    {
                        towar.Ilosc -= item.Towar.Ilosc;
                    }
                }
            }
        }

        private void comboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int tag = comboBox.SelectedIndex;
            switch (tag)
            {
                case 0: //nazwa
                    towary = new ObservableCollection<Towar>(towary.OrderBy(x => x.Nazwa).ToList());
                    break;
                case 1: //kod
                    towary = new ObservableCollection<Towar>(towary.OrderBy(x => x.Kod).ToList());
                    break;
            }
            lista.ItemsSource = towary;
        }
    }
}
