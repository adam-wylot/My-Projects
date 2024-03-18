using Shared;
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
using System.Windows.Shapes;

namespace SystemMagazynowyGUI.Okna
{
    /// <summary>
    /// Logika interakcji dla klasy DodajTowarWindow.xaml
    /// </summary>
    public partial class DodajTowarWindow : Window
    {
        private UserRepository userRepository;
        private int id;
        public DodajTowarWindow(UserRepository userRepository)
        {
            InitializeComponent();
            typesCb.ItemsSource = Enum.GetValues(typeof(TypyTowarow));
            this.userRepository = userRepository;
            id = -1;
        }
        public DodajTowarWindow(UserRepository userRepository, int id) : this(userRepository) //edycja
        {
            this.id = id;
            bt.Content = "Edytuj";

            Towar? towar = userRepository.GetTowar(id);
            if (towar != null)
            {
                kodTb.Text = towar.Kod.ToString();
                nazwaTb.Text = towar.Nazwa;
                gwarancjaTb.Text = $"{towar.Gwarancja.Day}.{towar.Gwarancja.Month}.{towar.Gwarancja.Year}";
                typesCb.SelectedItem = towar.Typ;
                iloscTb.Text = towar.Ilosc.ToString();
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (id == -1) //dodanie produktu
            {
                if (int.TryParse(kodTb.Text, out int kod) && int.TryParse(iloscTb.Text, out int ilosc) && DateTime.TryParse(gwarancjaTb.Text, out DateTime gwarancja) && typesCb.SelectedItem is TypyTowarow typ)
                {
                    if (userRepository.DodajTowar(kod, nazwaTb.Text, gwarancja, typ, ilosc))
                    {
                        MessageBox.Show("Pomyślnie dodano towar");
                    }
                    else
                    {
                        MessageBox.Show("Nie udało się dodać towaru!");
                    }
                }
                else
                {
                    MessageBox.Show("Nie udało się dodać towaru!");
                }
            }
            else //edycja
            {
                if (int.TryParse(kodTb.Text, out int kod) && int.TryParse(iloscTb.Text, out int ilosc) && DateTime.TryParse(gwarancjaTb.Text, out DateTime gwarancja) && typesCb.SelectedItem is TypyTowarow typ)
                {
                    if (userRepository.EdytujTowar(id, kod, nazwaTb.Text, gwarancja, typ, ilosc))
                    {
                        MessageBox.Show("Pomyślnie edytowano towar");
                    }
                    else
                    {
                        MessageBox.Show("Nie udało się zedytować towaru!");
                    }
                }
                else
                {
                    MessageBox.Show("Nie udało się zedytować towaru!");
                }
            }
        }
    }
}
