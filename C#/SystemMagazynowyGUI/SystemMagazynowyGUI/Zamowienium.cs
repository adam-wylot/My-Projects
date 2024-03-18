using System;
using System.Collections.Generic;

namespace SystemMagazynowyGUI
{
    public partial class Zamowienium
    {
        public Zamowienium()
        {
            Towaries = new HashSet<Towary>();
        }

        public int Id { get; set; }
        public string Imie { get; set; } = null!;
        public string Nazwisko { get; set; } = null!;
        public string Adres { get; set; } = null!;
        public int Dostarczenie { get; set; }
        public DateTime? DataRealizacji { get; set; }

        public virtual ICollection<Towary> Towaries { get; set; }
    }
}
