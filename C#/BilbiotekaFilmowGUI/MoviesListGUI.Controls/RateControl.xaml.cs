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

namespace MoviesListGUI.Controls
{
    public partial class RateControl : UserControl
    {
        private SolidColorBrush filledCirle = new SolidColorBrush(Colors.Red);
        private List<Ellipse> ellipses;
        private int value = 0;

        public Action<int> OnValueChanged { get; set; }

        public int Value
        {
            get { return value; }
            set
            {
                if (value >= 0 && value <= 5)
                {
                    this.value = value;
                    RefreshCircles();
                    if (OnValueChanged != null)
                    {
                        OnValueChanged(this.value);
                    }
                }
            }
        }

        public SolidColorBrush FilledCirle
        {
            get => filledCirle;
            set
            {
                filledCirle = value;
                RefreshCircles();
            }
        }

        public RateControl()
        {
            //LINQ
            InitializeComponent();
            ellipses = new List<Ellipse>();
            foreach (var uiElement in circles.Children)
            {
                if (uiElement is Ellipse e)
                {
                    ellipses.Add(e);
                }
            }
        }

        private void Ellipse_MouseEnter(object sender, MouseEventArgs e)
        {
            var circle = sender as Ellipse;
            int index = int.Parse(circle.Tag.ToString());
            for (int i = 0; i <= index; i++)
            {
                ellipses[i].Fill = filledCirle;
            }
            for (int i = index + 1; i < ellipses.Count; i++)
            {
                ellipses[i].Fill = new SolidColorBrush(Colors.White);
            }
        }

        private void RefreshCircles()
        {
            for (int i = 0; i < value; i++)
            {
                ellipses[i].Fill = filledCirle;
            }
            for (int i = value; i < ellipses.Count; i++)
            {
                ellipses[i].Fill = new SolidColorBrush(Colors.White);
            }
        }

        private void Ellipse_MouseLeave(object sender, MouseEventArgs e)
        {
            RefreshCircles();
        }

        private void Ellipse_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Value = int.Parse((sender as Ellipse).Tag.ToString()) + 1;
        }
    }
}