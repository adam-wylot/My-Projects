using Shared;

namespace SystemMagazynowyMVC.Services
{
    public interface IBasketService
    {
        Towar this[int index] { get; }

        int Count { get; }

        List<Towar> Towary { get; }

        void Add(Towar t);

        void Remove(Towar t);
    }
}