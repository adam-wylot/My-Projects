using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Shared
{
    public enum TypyTowarow
    {
        telefon,
        laptop,
        klawiatura,
        myszka,
        sluchawki
    }
    public class Towar
    {
        public int Id { get; set; }
        public int Kod { get; set; }
        public string Nazwa { get; set; }
        public DateTime Gwarancja { get; set; }
        public TypyTowarow Typ { get; set; }
        public int Ilosc { get; set; }

        public override string? ToString()
        {
            return $"[{Kod}] {Nazwa} (Ilość: {Ilosc})";
        }
    }
}
