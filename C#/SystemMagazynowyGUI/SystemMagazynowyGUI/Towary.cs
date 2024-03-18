using System;
using System.Collections.Generic;

namespace SystemMagazynowyGUI
{
    public partial class Towary
    {
        public int Id { get; set; }
        public int Kod { get; set; }
        public string Nazwa { get; set; } = null!;
        public DateTime Gwarancja { get; set; }
        public int Typ { get; set; }
        public int Ilosc { get; set; }
        public int? ZamowienieId { get; set; }

        public virtual Zamowienium? Zamowienie { get; set; }
    }
}
