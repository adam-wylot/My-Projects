using System;
using System.Xml.Serialization;

namespace ProgramKalkulacyjnyGUI
{
    [XmlInclude(typeof(Worker))]
    public class Worker
    {
        //Zmienne
        private Month? month;
        private int monthIndex;
        private int sectionIndex;
        private string? name;
        private double[] values;

        internal Month? Month => month;
        public int MonthIndex
        {
            get => monthIndex;
            set
            {
                monthIndex = value;
                if (monthIndex < 12)
                {
                    month = ProgramRepository.Repository?.Months[monthIndex] as Month;
                }
            }
        }
        public int SectionIndex { get => sectionIndex; set => sectionIndex = value; }
        public string? Name { get => name; set => name = value; }
        public double[] Values { get => values; set => values = value; }

        //Konstruktory
        public Worker()
        {
            monthIndex = new int();
            sectionIndex = new int();
            values = Array.Empty<double>();
        }
        public Worker(Section section)
        {
            MonthIndex = section.TabBelonging.Index;
            sectionIndex = section.Index;
            values = new double[ProgramRepository.Repository.ColumnsAmount];

            if (Month != null)
            {
                OverrallSummary.AddWorker(this);
            }
        }
    }
}
