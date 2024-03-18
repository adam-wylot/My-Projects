using System;
using System.Windows;
using System.Windows.Controls;

namespace ProgramKalkulacyjnyGUI
{
    internal class RowGUI
    {
        private readonly Row data;

        private readonly Grid nameGrid;
        private readonly Grid grid;
        private readonly RowDefinition nameGridRowDef;
        private readonly RowDefinition rowDef;

        private TextBox numberTb;
        private TextBox nameTb;
        private TextBox[] textBoxes;
        private readonly Button deleteRowButton;

        public Grid NameGrid => nameGrid;

        public Grid Grid => grid;

        public RowDefinition NameGridRowDef => nameGridRowDef;

        public RowDefinition RowDef => rowDef;

        public TextBox NumberTb { get => numberTb; set => numberTb = value; }
        public TextBox NameTb { get => nameTb; set => nameTb = value; }
        public TextBox[] TextBoxes { get => textBoxes; set => textBoxes = value; }

        public Button DeleteRowButton => deleteRowButton;

        public RowGUI(Row dataRow, double rowHeight, Grid nameGrid, Grid grid)
        {
            data = dataRow;

            this.nameGrid = nameGrid;
            this.grid = grid;
            numberTb = new TextBox();
            nameTb = new TextBox();
            textBoxes = new TextBox[ProgramRepository.Repository.ColumnsAmount];

            nameGridRowDef = new RowDefinition();
            rowDef = new RowDefinition();
            deleteRowButton = new Button();

            RowSetter(rowHeight);
        }

        //Metody
        public void ChangeObjectsRow(int value)
        {
            Grid.SetRow(deleteRowButton, Grid.GetRow(deleteRowButton) + value);
            Grid.SetRow(numberTb, Grid.GetRow(numberTb) + value);
            Grid.SetRow(nameTb, Grid.GetRow(nameTb) + value);

            for (int i = 0; i < textBoxes.Length; i++)
            {
                Grid.SetRow(textBoxes[i], Grid.GetRow(textBoxes[i]) + value);
            }
        }
        private void RowSetter(double rowHeight)
        {
            rowDef.Height = new GridLength(rowHeight, GridUnitType.Pixel);
            grid.RowDefinitions.Insert(data.Index + 1, RowDef);
            nameGridRowDef.Height = new GridLength(rowHeight, GridUnitType.Pixel);
            nameGrid.RowDefinitions.Insert(data.Index + 1, nameGridRowDef);

            TextBoxesSetter();

            deleteRowButton.Content = "-";
            deleteRowButton.FontSize = 16;
            deleteRowButton.FontWeight = FontWeights.Bold;
            deleteRowButton.Click += Delete_Row_Button_Click;
            deleteRowButton.MouseLeave += LP_MouseLeave;
            deleteRowButton.Visibility = Visibility.Hidden;
            Grid.SetRow(deleteRowButton, data.Index + 1);
            Grid.SetColumn(deleteRowButton, 0);
            nameGrid.Children.Add(deleteRowButton);
        }
        private void TextBoxesSetter()
        {
            numberTb.Text = (data.Index + 1).ToString();
            numberTb.IsReadOnly = true;
            numberTb.FontSize = 15;
            numberTb.FontWeight = FontWeights.Bold;
            numberTb.TextAlignment = TextAlignment.Center;
            numberTb.MouseEnter += LP_MouseEnter;
            Grid.SetRow(numberTb, data.Index + 1);
            Grid.SetColumn(numberTb, 0);
            nameGrid.Children.Add(numberTb);

            nameTb.TextChanged += Name_Changed;
            nameTb.FontSize = 15;
            Grid.SetRow(nameTb, data.Index + 1);
            Grid.SetColumn(nameTb, 1);
            nameGrid.Children.Add(nameTb);

            //miejsce kalkulacyjne
            for (int i = 0; i < ProgramRepository.Repository.ColumnsAmount; i++)
            {
                textBoxes[i] = new TextBox
                {
                    FontSize = 15
                };
                if (i < ProgramRepository.Repository.PropertiesTbAmount || i == 15 || i == 22)
                {
                    textBoxes[i].TextChanged += TextChangedEventHandler;
                }
                //tb.VerticalAlignment = VerticalAlignment.Center;
                Grid.SetRow(textBoxes[i], data.Index + 1);
                Grid.SetColumn(textBoxes[i], i);
                grid.Children.Add(textBoxes[i]);
            }
        }

        public void Name_Changed(object sender, TextChangedEventArgs e)
        {
            data.ChangeName(nameTb.Text);
        }
        public void TextChangedEventHandler(object sender, TextChangedEventArgs e)
        {
            if (sender is TextBox tb)
            {
                if (double.TryParse(tb.Text, out double temp))
                {
                    Math.Round(temp, 2, MidpointRounding.AwayFromZero);
                    data.ChangeValue(Grid.GetColumn(tb), temp);

                    switch (ProgramRepository.Repository.FileType)
                    {
                        case FileTypes.kierowcy:
                            data.CalculateForDrivers();
                            break;
                        case FileTypes.montaz:
                            data.CalculateForMontage();
                            break;
                        case FileTypes.montaz_zl:
                            //data.CalculateForMontageZL();
                            break;
                    }

                    //section.TabBelonging.Summary.UpdateSummary(Grid.GetColumn(tb));
                    data.Section.TabBelonging.Summary.UpdateSummary();
                }
            }
        }
        private void LP_MouseEnter(object sender, System.Windows.Input.MouseEventArgs e)
        {
            deleteRowButton.Visibility = Visibility.Visible;
        }
        private void LP_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            deleteRowButton.Visibility = Visibility.Hidden;
        }
        private void Delete_Row_Button_Click(object sender, RoutedEventArgs e)
        {
            RemoveThisRow();
        }
        public void RemoveThisRow()
        {
            //Usuwanie obiektów z wiersza
            nameGrid.Children.Remove(deleteRowButton);
            nameGrid.Children.Remove(numberTb);
            nameGrid.Children.Remove(nameTb);
            for (int i = 0; i < textBoxes.Length; i++)
            {
                grid.Children.Remove(textBoxes[i]);
            }

            nameGrid.RowDefinitions.Remove(nameGridRowDef);
            grid.RowDefinitions.Remove(rowDef);

            Grid.SetRow(data.Section.Gui.AddBt, Grid.GetRow(data.Section.Gui.AddBt) - 1);
            Grid.SetRow(data.Section.Gui.PenultimentRow, Grid.GetRow(data.Section.Gui.PenultimentRow) - 1);

            Grid.SetRow(data.Section.Gui.LastRowNG, Grid.GetRow(data.Section.Gui.LastRowNG) - 1);
            Grid.SetRow(data.Section.Gui.LastRow, Grid.GetRow(data.Section.Gui.LastRow) - 1);

            data.RemoveRow();
        }
        public void ReducePositionArr()
        {
            data.Index--;
            numberTb.Text = (data.Index + 1).ToString();
            ChangeObjectsRow(-1);
        }

    }
}
