using System;
using System.Globalization;
using System.Windows.Controls;

namespace ProgramKalkulacyjnyGUI
{
    public class Row
    {
        private RowGUI gui;

        private int index;
        private Section section;
        private Worker worker;

        public int Index { get => index; set => index = value; }
        public Section Section { get => section; set => section = value; }
        public Worker Worker { get => worker; set => worker = value; }
        internal RowGUI Gui { get => gui; }

        //Konstruktory
        public Row(int index, Section section, double rowHeight, Grid nameGrid, Grid grid)
        {
            this.index = index;
            this.section = section;

            gui = new RowGUI(this, rowHeight, nameGrid, grid);

            worker = new Worker(section);
            section.TabBelonging.Summary.ChangeLP(1);

        }

        //Metody
        public void ChangeName(string? s)
        {
            worker.Name = s;
        }
        public void ChangeValue(int indx, double value)
        {
            worker.Values[indx] = value;
        }
        internal void RemoveRow()
        {
            section.Lp--;
            section.TabBelonging.Summary.ChangeLP(-1);
            ProgramRepository.ChangeCalculationSpaceSize(section.Gui.SectionRowDef, -1.0 * gui.RowDef.Height.Value);
            Section.Rows.Remove(this);

            //Przesuwanie pozostałych obiektów
            for (int i = index; i < section.Rows.Count; i++)
            {
                section.Rows[i].Gui.ReducePositionArr();
            }

            OverrallSummary.RemoveWorker(worker);
            section.TabBelonging.Summary.UpdateSummary();
        }

        //Obliczeniowe
        private void SetValue(int index, double value)
        {
            value = Math.Round(value, 2, MidpointRounding.AwayFromZero);
            worker.Values[index] = value;
            gui.TextBoxes[index].Text = value.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
        }
        internal void CalculateForDrivers()
        {
            double a = 0;
            double b;

            //Ogolem przychod
            for (int i = 1; i < ProgramRepository.Repository.PropertiesTbAmount; i++)
            {
                if (double.TryParse(gui.TextBoxes[i].Text, out b))
                {
                    a += b;
                }
            }
            if (double.TryParse(gui.TextBoxes[2].Text, out b))
            {
                a -= b;
            }
            SetValue(8, a);

            //Podst. wym. skl.
            a = 0;
            if (double.TryParse(gui.TextBoxes[8].Text, out b))
            {
                a += b;
            }
            if (double.TryParse(gui.TextBoxes[7].Text, out b))
            {
                a -= b;
            }
            if (double.TryParse(gui.TextBoxes[2].Text, out b))
            {
                a += b;
            }
            SetValue(9, a);

            //Emerytalne
            if (double.TryParse(gui.TextBoxes[9].Text, out b))
            {
                SetValue(10, b * 0.0976);
            }
            else
            {
                gui.TextBoxes[10].Text = "Błąd!";
            }

            //1,5% rentowe
            if (double.TryParse(gui.TextBoxes[9].Text, out b))
            {
                SetValue(11, b * 0.015);
            }
            else
            {
                gui.TextBoxes[11].Text = "Błąd!";
            }

            //6,5% rentowe
            if (double.TryParse(gui.TextBoxes[9].Text, out b))
            {
                SetValue(12, b * 0.065);
            }
            else
            {
                gui.TextBoxes[12].Text = "Błąd!";
            }

            //2,45% chorobowe
            if (double.TryParse(gui.TextBoxes[9].Text, out b))
            {
                SetValue(13, b * 0.0245);
            }
            else
            {
                gui.TextBoxes[13].Text = "Błąd!";
            }

            //Skladki razem
            a = 0;
            if (double.TryParse(gui.TextBoxes[10].Text, out b))
            {
                a += b;
            }
            if (double.TryParse(gui.TextBoxes[11].Text, out b))
            {
                a += b;
            }
            if (double.TryParse(gui.TextBoxes[13].Text, out b))
            {
                a += b;
            }
            SetValue(14, a);

            //Podst. naliczenia skl. zdrowotnej
            if (double.TryParse(gui.TextBoxes[8].Text, out a) && double.TryParse(gui.TextBoxes[14].Text, out b))
            {
                a -= b;
                SetValue(16, a);
            }
            else
            {
                gui.TextBoxes[16].Text = "Błąd!";
            }

            //Ulga dla klasy sredniej wariant a
            if (double.TryParse(gui.TextBoxes[8].Text, out b))
            {
                if (b >= 5701 && b <= 8549)
                {
                    b = (0.0668 * b - 380.5) / 0.17;
                }
                else if (b > 8549 && b <= 11141)
                {
                    b = (-0.0735 * b + 819.08) / 0.17;
                }
                else
                {
                    b = 0;
                }
                SetValue(17, b);
            }
            else
            {
                gui.TextBoxes[17].Text = "Błąd!";
            }

            //Podst. naliczenia podatku dochodowego
            if (double.TryParse(gui.TextBoxes[16].Text, out a) && double.TryParse(gui.TextBoxes[15].Text, out b) && double.TryParse(gui.TextBoxes[17].Text, out double c))
            {
                a -= b + c;

                a = Math.Round(a, 0, MidpointRounding.AwayFromZero);
                Worker.Values[18] = a;
                gui.TextBoxes[18].Text = a.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
            }
            else
            {
                gui.TextBoxes[18].Text = "Błąd!";
            }

            //Zaliczka na podatek dochodowy
            if (double.TryParse(gui.TextBoxes[18].Text, out b))
            {
                b = Math.Round(b * 0.17 - 425, 0, MidpointRounding.AwayFromZero);
                if (b < 0)
                {
                    b = 0;
                }
                Worker.Values[19] = b;
                gui.TextBoxes[19].Text = b.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
            }
            else
            {
                gui.TextBoxes[19].Text = "Błąd!";
            }

            //Skladka zdrowotna
            if (double.TryParse(gui.TextBoxes[16].Text, out b))
            {
                SetValue(20, b * 0.09);
            }
            else
            {
                gui.TextBoxes[20].Text = "Błąd!";
            }

            //Do wypłaty
            if (double.TryParse(gui.TextBoxes[16].Text, out a) && double.TryParse(gui.TextBoxes[19].Text, out b) && double.TryParse(gui.TextBoxes[20].Text, out c))
            {
                a -= b + c;
                SetValue(21, a);
            }
            else
            {
                gui.TextBoxes[21].Text = "Błąd!";
            }

            //skladka na ubezpieczenia wypadkowe
            if (double.TryParse(gui.TextBoxes[9].Text, out a))
            {
                SetValue(23, a * 0.0096);
            }
            else
            {
                gui.TextBoxes[23].Text = "Błąd!";
            }
        }
        internal void CalculateForMontage()
        {
            double a;
            double b;

            //Ogolem przychod
            if (double.TryParse(gui.TextBoxes[0].Text, out a))
            {
                if (double.TryParse(gui.TextBoxes[1].Text, out b))
                {
                    SetValue(8, a - b);
                }
            }

            //Wymiar skladek ubezpieczen spolecznych
            a = 0;
            for (int i = 3; i <= 6; i++)
            {
                if (double.TryParse(gui.TextBoxes[i].Text, out b))
                {
                    a += b;
                }
                else
                {
                    gui.TextBoxes[9].Text = "Błąd!";
                    break;
                }
            }
            SetValue(9, a);

            //Emerytalne
            if (double.TryParse(gui.TextBoxes[9].Text, out a))
            {
                SetValue(10, a * 0.0976);
            }
            else
            {
                gui.TextBoxes[10].Text = "Błąd!";
            }

            //1,5% rentowe
            if (double.TryParse(gui.TextBoxes[9].Text, out a))
            {
                SetValue(11, a * 0.015);
            }
            else
            {
                gui.TextBoxes[11].Text = "Błąd!";
            }

            //6,5% rentowe
            if (double.TryParse(gui.TextBoxes[9].Text, out a))
            {
                SetValue(12, a * 0.065);
            }
            else
            {
                gui.TextBoxes[12].Text = "Błąd!";
            }

            //2,45% chorobowe
            if (double.TryParse(gui.TextBoxes[9].Text, out a))
            {
                SetValue(13, a * 0.0245);
            }
            else
            {
                gui.TextBoxes[13].Text = "Błąd!";
            }

            //Skladki razem
            a = 0;
            if (double.TryParse(gui.TextBoxes[10].Text, out b))
            {
                a += b;
            }
            if (double.TryParse(gui.TextBoxes[11].Text, out b))
            {
                a += b;
            }
            if (double.TryParse(gui.TextBoxes[13].Text, out b))
            {
                a += b;
            }
            SetValue(14, a);

            //Podst. naliczenia skl. zdrowotnej
            if (double.TryParse(gui.TextBoxes[9].Text, out a) && double.TryParse(gui.TextBoxes[14].Text, out b))
            {
                a -= b;
                SetValue(16, a);
            }
            else
            {
                gui.TextBoxes[16].Text = "Błąd!";
            }

            //Ulga dla klasy sredniej wariant a
            if (double.TryParse(gui.TextBoxes[8].Text, out a))
            {
                if (a >= 5701 && a <= 8549)
                {
                    a = (0.0668 * a - 380.5) / 0.17;
                }
                else if (b > 8549 && a <= 11141)
                {
                    a = (-0.0735 * a + 819.08) / 0.17;
                }
                else
                {
                    a = 0;
                }
                SetValue(17, a);
            }
            else
            {
                gui.TextBoxes[17].Text = "Błąd!";
            }

            //Podst. naliczenia podatku dochodowego
            if (double.TryParse(gui.TextBoxes[8].Text, out a) && double.TryParse(gui.TextBoxes[14].Text, out b) && double.TryParse(gui.TextBoxes[15].Text, out double c) && double.TryParse(gui.TextBoxes[17].Text, out double d))
            {
                a -= b + c + d;

                a = Math.Round(a, 0, MidpointRounding.AwayFromZero);
                Worker.Values[18] = a;
                gui.TextBoxes[18].Text = a.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
            }
            else
            {
                gui.TextBoxes[18].Text = "Błąd!";
            }

            //Zaliczka na podatek dochodowy
            if (double.TryParse(gui.TextBoxes[18].Text, out a))
            {
                a = Math.Round(a * 0.17 - 425, 0, MidpointRounding.AwayFromZero);
                if (a < 0)
                {
                    a = 0;
                }
                Worker.Values[19] = a;
                gui.TextBoxes[19].Text = a.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
            }
            else
            {
                gui.TextBoxes[19].Text = "Błąd!";
            }

            //Skladka zdrowotna
            if (double.TryParse(gui.TextBoxes[16].Text, out a))
            {
                SetValue(20, a * 0.09);
            }
            else
            {
                gui.TextBoxes[20].Text = "Błąd!";
            }

            //Do wypłaty
            if (double.TryParse(gui.TextBoxes[0].Text, out a) && double.TryParse(gui.TextBoxes[14].Text, out b) && double.TryParse(gui.TextBoxes[19].Text, out c) && double.TryParse(gui.TextBoxes[20].Text, out d))
            {
                a -= b + c + d;
                SetValue(21, a);
            }
            else
            {
                gui.TextBoxes[21].Text = "Błąd!";
            }

            //skladka na ubezpieczenia wypadkowe
            if (double.TryParse(gui.TextBoxes[9].Text, out a))
            {
                SetValue(23, a * 0.0096);
            }
            else
            {
                gui.TextBoxes[23].Text = "Błąd!";
            }
        }
        internal void CalculateForMontageZL()
        {
            double a;
            double b;

            //Podstawa opodatkowania
            if (double.TryParse(gui.TextBoxes[0].Text, out a))
            {
                if (double.TryParse(gui.TextBoxes[1].Text, out b))
                {
                    SetValue(7, a - b);
                }
            }

            //Podstawa wymiaru skladek ubezpieczen spolecznych
            a = 0;
            for (int i = 2; i <= 5; i++)
            {
                if (double.TryParse(gui.TextBoxes[i].Text, out b))
                {
                    a += b;
                }
                else
                {
                    gui.TextBoxes[8].Text = "Błąd!";
                    break;
                }
            }
            SetValue(8, a);

            //Emerytalne
            if (double.TryParse(gui.TextBoxes[8].Text, out a))
            {
                SetValue(9, a * 0.0976);
            }
            else
            {
                gui.TextBoxes[9].Text = "Błąd!";
            }

            //1,5% rentowe
            if (double.TryParse(gui.TextBoxes[8].Text, out a))
            {
                SetValue(10, a * 0.015);
            }
            else
            {
                gui.TextBoxes[10].Text = "Błąd!";
            }

            //6,5% rentowe
            if (double.TryParse(gui.TextBoxes[8].Text, out a))
            {
                SetValue(11, a * 0.065);
            }
            else
            {
                gui.TextBoxes[11].Text = "Błąd!";
            }

            ////2,45% chorobowe
            //if (double.TryParse(gui.TextBoxes[9].Text, out a))
            //{
            //    SetValue(12, a * 0.0245);
            //}
            //else
            //{
            //    gui.TextBoxes[12].Text = "Błąd!";
            //}

            //Skladki razem
            a = 0;
            if (double.TryParse(gui.TextBoxes[9].Text, out b))
            {
                a += b;
            }
            if (double.TryParse(gui.TextBoxes[10].Text, out b))
            {
                a += b;
            }
            if (double.TryParse(gui.TextBoxes[12].Text, out b))
            {
                a += b;
            }
            SetValue(13, a);

            //Koszty uzyskania przychodu
            if (double.TryParse(gui.TextBoxes[7].Text, out a) && double.TryParse(gui.TextBoxes[13].Text, out b))
            {
                SetValue(14, (a - b) * 0.2);
            }

            //Podst. naliczenia skl. zdrowotnej
            if (double.TryParse(gui.TextBoxes[8].Text, out a) && double.TryParse(gui.TextBoxes[13].Text, out b))
            {
                a -= b;
                SetValue(15, a);
            }
            else
            {
                gui.TextBoxes[15].Text = "Błąd!";
            }

            //Podst. naliczenia podatku dochodowego
            if (double.TryParse(gui.TextBoxes[7].Text, out a) && double.TryParse(gui.TextBoxes[13].Text, out b) && double.TryParse(gui.TextBoxes[14].Text, out double c))
            {
                a -= b + c;

                a = Math.Round(a, 0, MidpointRounding.AwayFromZero);
                Worker.Values[16] = a;
                gui.TextBoxes[16].Text = a.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
            }
            else
            {
                gui.TextBoxes[16].Text = "Błąd!";
            }

            //Zaliczka na podatek dochodowy
            if (double.TryParse(gui.TextBoxes[16].Text, out a))
            {
                a = Math.Round(a * 0.17, 0, MidpointRounding.AwayFromZero);
                if (a < 0)
                {
                    a = 0;
                }
                Worker.Values[17] = a;
                gui.TextBoxes[17].Text = a.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
            }
            else
            {
                gui.TextBoxes[17].Text = "Błąd!";
            }

            //Skladka zdrowotna
            if (double.TryParse(gui.TextBoxes[15].Text, out a))
            {
                SetValue(18, a * 0.09);
            }
            else
            {
                gui.TextBoxes[18].Text = "Błąd!";
            }

            //Do wypłaty
            if (double.TryParse(gui.TextBoxes[0].Text, out a) && double.TryParse(gui.TextBoxes[13].Text, out b) && double.TryParse(gui.TextBoxes[17].Text, out c) && double.TryParse(gui.TextBoxes[18].Text, out double d))
            {
                a -= b + c + d;
                SetValue(19, a);
            }
            else
            {
                gui.TextBoxes[19].Text = "Błąd!";
            }

            //skladka na ubezpieczenia wypadkowe
            if (double.TryParse(gui.TextBoxes[8].Text, out a))
            {
                SetValue(20, a * 0.0096);
            }
            else
            {
                gui.TextBoxes[20].Text = "Błąd!";
            }
        }
    }
}
