using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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

namespace Memory_masterGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private char[] chars = { '!', '@', '#', '$', '%', '^', '&', '*' }; //max 8 znakow
        private char[] array = new char[16];
        private bool shown = false;
        private Button button1;
        private Button button2;

        public MainWindow()
        {
            InitializeComponent();
            Reset();
        }

        //Metody
        private void Draw()
        {
            Random rand = new Random();
            List<char> leftChars = new List<char>();

            foreach (var item in chars)
            {
                leftChars.Add(item);
                leftChars.Add(item);
            }

            for (int i = 0; i < 16; i++)
            {
                int temp = rand.Next(0, leftChars.Count);
                array[i] = leftChars[temp];
                leftChars.RemoveAt(temp);
            }
        }
        private int GetPoints()
        {
            string[] arr = display.Text.Split(" ");
            return int.Parse(arr[1]);
        }
        private void AddPoint()
        {
            int points = GetPoints()+1;
            display.Text = $"Punkty: {points}";

            if(points == 8)
            {
                display.Text = "Wygrales!";
            }
        }
        private void Reset()
        {
            //Pytanie - Jest jakas tablica zawierajaca wszystkie przyciski?
            List<Button> buttons = buttonsGrid.Children.Cast<Button>().ToList();
            for (int i = 0; i < buttons.Count; i++)
            {
                buttons[i].Tag = i.ToString();
                buttons[i].Content = "";
            }

            Draw();
            display.Text = "Punkty: 0";
        }

        //Przyciski
        private void Button_Click_Reset(object sender, RoutedEventArgs e)
        {
            Reset();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button b = sender as Button;
            if (b != null && b.Tag.ToString() != "blocked" )
            {
                b.Content = array[int.Parse(b.Tag.ToString())]; //Pytanie - (Rzutowanie nie dzialalo) Czy istnieje jakas sprytniejsza mozliwosc konwersji wartosci tagu na liczbe calkowita?

                if(!shown)
                {
                    if (button1 != null && button2 != null)
                    {
                        button1.Content = "";
                        button2.Content = "";
                        b.Content = array[int.Parse(b.Tag.ToString())];
                    }

                    button1 = b;
                    shown = true;
                }
                else
                {
                    char ch1 = array[int.Parse(b.Tag.ToString())];
                    char ch2 = array[int.Parse(button1.Tag.ToString())];
                    shown = false;

                    if(ch1 == ch2 && b.Tag.ToString() != button1.Tag.ToString())
                    {
                        AddPoint();

                        button1.Tag = "blocked";
                        b.Tag = "blocked";

                        button1 = null;
                        button2 = null;
                    }
                    else
                    {
                        button2 = b;
                    }
                }
            }
        }
    }
}
