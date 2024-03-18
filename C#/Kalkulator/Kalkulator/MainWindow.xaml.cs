using System;
using System.Collections.Generic;
using System.Globalization;
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

namespace Kalkulator
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    /// 
    enum Operations
    {
        adding = 1,
        subtract,
        multiplication,
        division,
        exponentiation,
        root
    };
    public partial class MainWindow : Window
    {
        private bool splitted = false;
        private bool switch_ = true;

        private char lastOperation = '=';

        private double a;
        private double b;

        private Operations operation;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Calculate()
        {
            switch_ = true;
            splitted = false;

            if (lastOperation != '=')
                b = double.Parse(display.Text);

            switch ((int)operation)
            {
                case 1:
                    a += b;
                    break;
                case 2:
                    a -= b;
                    break;
                case 3:
                    a *= b;
                    break;
                case 4:
                    a /= b; ;
                    break;
                case 5:
                    a = Math.Pow(a,b);
                    break;
                case 6:
                    double result = Math.Sqrt(double.Parse(display.Text));
                    display.Text = result.ToString();
                    return;
            }
            display.Text = a.ToString();
        }
        
        private void Button_Click_Num(object sender, RoutedEventArgs e) //sender - element GUI (np przycisk, TextBox) ktory aktualnie wykonuje ta metode
        {
            Button button = sender as Button;
            if(button != null)
            {
                if (switch_)
                {
                    display.Text = button.Tag.ToString();
                    switch_ = false;
                }
                else
                    display.Text += button.Tag.ToString();
            }
        }

        private void Button_Click_power(object sender, RoutedEventArgs e)
        {
            if (lastOperation != '=')
                Calculate();
            operation = Operations.exponentiation;
            a = double.Parse(display.Text);
            switch_ = true;
            lastOperation = 'p';
        }
        private void Button_Click_sqrt(object sender, RoutedEventArgs e)
        {
            if (lastOperation != '=' && lastOperation != 's')
                Calculate();
            operation = Operations.root;
            double result = Math.Sqrt(double.Parse(display.Text));
            display.Text = result.ToString();
            lastOperation = 's';
        }

        private void Button_Click_div(object sender, RoutedEventArgs e)
        {
            if (lastOperation != '=')
                Calculate();
            operation = Operations.division;
            a = double.Parse(display.Text);
            switch_ = true;
            lastOperation = '/';
        }

        private void Button_Click_times(object sender, RoutedEventArgs e)
        {
            if (lastOperation != '=')
                Calculate();
            operation = Operations.multiplication;
            a = double.Parse(display.Text);
            switch_ = true;
            lastOperation = '*';
        }

        private void Button_Click_minus(object sender, RoutedEventArgs e)
        {
            if (lastOperation != '=')
                Calculate();
            operation = Operations.subtract;
            a = double.Parse(display.Text);
            switch_ = true;
            lastOperation = '-';
        }

        private void Button_Click_plus(object sender, RoutedEventArgs e)
        {
            if (lastOperation != '=')
                Calculate();
            operation = Operations.adding;
            a = double.Parse(display.Text, CultureInfo.InvariantCulture); //InvariantCulture - parsowanie odberdzie sie z uzyciem standardowego znaku dla speratora dziesietnego czyli kropka
            switch_ = true;
            lastOperation = '+';
        }

        private void Button_Click_eq(object sender, RoutedEventArgs e)
        {
            Calculate();
            lastOperation = '=';
        }

        private void Button_Click_split(object sender, RoutedEventArgs e)
        {
            if(!splitted)
            {
                splitted = true;
                switch_ = false;
                display.Text += ".";
            }
        }

        private void Button_Click_clear(object sender, RoutedEventArgs e)
        {
            switch_ = true;
            splitted = false;
            lastOperation = '=';
            display.Text = "0";
        }

        private void Button_Click_back(object sender, RoutedEventArgs e)
        {
            int dl = display.Text.Count()-1;

            if (display.Text[dl].Equals('.'))
                splitted = false;

            if (dl > 0)
                display.Text = display.Text.Remove(dl, 1);
            else
            {
                switch_ = true;
                display.Text = "0";
            }
        }
    }
}
