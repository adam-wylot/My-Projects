using Shared;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SystemMagazynowyGUI
{
    public class UserRepository
    {
        private readonly DataContext context;

        public DataContext Context => context;

        public UserRepository(DataContext context)
        {
            this.context = context;
        }

        //Metody
        public bool DodajTowar(int kod, string nazwa, DateTime gwarancja, TypyTowarow typ, int ilosc)
        {
            var towar = context.Towary.FirstOrDefault(x => x.Kod == kod);
            if (towar != null)
            {
                return false;
            }

            towar = new Towar
            {
                Kod = kod,
                Nazwa = nazwa,
                Gwarancja = gwarancja,
                Typ = typ,
                Ilosc = ilosc
            };
            context.Towary.Add(towar);
            context.SaveChanges();
            return true;
        }
        public bool EdytujTowar(int id, int kod, string nazwa, DateTime gwarancja, TypyTowarow typ, int ilosc)
        {
            var towar = context.Towary.FirstOrDefault(x => x.Id == id);
            if (towar == null)
            {
                return false;
            }

            towar.Kod = kod;
            towar.Nazwa = nazwa;
            towar.Gwarancja = gwarancja;
            towar.Typ = typ;
            towar.Ilosc = ilosc;
            return true;
        }
        public List<Towar> GetTowary()
        {
            return context.Towary.ToList();
        }
        public List<Zamowienie> GetZamowienia()
        {
            return context.Zamowienia.ToList();
        }
        public Towar? GetTowar(int id)
        {
            return context.Towary.FirstOrDefault(x => x.Id == id);
        }
        public Towar? GetTowarByKod(int kod)
        {
            return context.Towary.FirstOrDefault(x => x.Kod == kod);
        }
        public bool UsunTowar(Towar towar)
        {
            var towar_ = context.ZamowieniaTowary.FirstOrDefault(x => x.Towar.Kod == towar.Kod);
            if (towar_ != null)
            {
                return false;
            }

            context.Towary.Remove(towar);
            context.SaveChanges();
            return true;
        }
    }
}
