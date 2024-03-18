using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Shared
{
    public enum SposobDostarczenia
    {
        kurier,
        paczkomat,
        punktPocztowy
    }
    public class Zamowienie
    {
        public int Id { get; set; }
        public DateTime DataZamowienia = DateTime.Now;
        public string Imie { get; set; }
        public string Nazwisko { get; set; }
        public string Adres { get; set; }
        public List<Towar> Towary { get; set; }
        public SposobDostarczenia Dostarczenie { get; set; }
        public DateTime? DataRealizacji { get; set; }

        public override string? ToString()
        {
            if (DataRealizacji == null)
            {
                return $"[{DataZamowienia}] {Imie} {Nazwisko}";
            }
            else
            {
                return $"[{DataZamowienia}] {Imie} {Nazwisko} - Zrealizowano";
            }
        }
    }
}
