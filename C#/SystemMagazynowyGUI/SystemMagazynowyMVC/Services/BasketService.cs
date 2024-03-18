using Shared;
using System.Linq;

namespace SystemMagazynowyMVC.Services
{
    public class BasketService : IBasketService
    {
        private static List<Towar> towary = new List<Towar>();

        public void Add(Towar t)
        {
            towary.Add(t);
        }

        public void Remove(Towar t)
        {
            var towar = towary.FirstOrDefault(x => x.Id == t.Id);
            if(towar == null)
            {
                return;
            }
            towary.Remove(towar);
        }

        public int Count => towary.Count;
        public List<Towar> Towary => towary;
        public Towar this[int index]
        {
            get { return towary[index]; }
        }
    }
}
