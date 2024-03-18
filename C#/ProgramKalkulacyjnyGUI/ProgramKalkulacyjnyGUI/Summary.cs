using System;
using System.Globalization;

namespace ProgramKalkulacyjnyGUI
{
    public class Summary
    {
        //Zmienne
        private SummaryGUI gui;

        private Tab tab;
        private int lp;

        internal SummaryGUI Gui { get => gui; }
        public Tab Tab => tab;
        public int Lp { get => lp; set => lp = value; }

        //Konstruktory
        public Summary(Tab tab)
        {
            this.tab = tab;
            lp = 0;

            gui = new SummaryGUI(this);
        }

        //Metody
        public void ChangeLP(int amount)
        {
            lp += amount;
            if (lp < 0)
            {
                lp = 0;
            }
            gui.WorkersNumTb.Text = "=" + lp.ToString();
        }
        public void UpdateSummary()
        {
            double sum;
            double temp;

            //Wszystko z podsumowania
            for (int i = 0; i < gui.SummaryTbs.Length; i++)
            {
                sum = 0;
                foreach (var section in tab.Sections)
                {
                    foreach (var row in section.Rows)
                    {
                        if (double.TryParse(row.Gui.TextBoxes[i].Text, out temp))
                        {
                            temp = Math.Round(temp, 2, MidpointRounding.AwayFromZero);
                            sum += temp;
                        }
                    }
                }

                sum = Math.Round(sum, 2, MidpointRounding.AwayFromZero);
                gui.SummaryTbs[i].Text = "=" + sum.ToString("N2", CultureInfo.CreateSpecificCulture("pl-PL"));
            }
        }
    }
}
