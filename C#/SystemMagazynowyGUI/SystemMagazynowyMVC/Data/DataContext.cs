using Microsoft.EntityFrameworkCore;
using Shared;

namespace SystemMagazynowyMVC.Data
{
    public class DataContext : DbContext
    {
        public DataContext(DbContextOptions<DataContext> options) : base(options)
        {

        }

        public DbSet<Towar> Towary { get; set; }
        public DbSet<Zamowienie> Zamowienia { get; set; }
        public DbSet<ZamowienieTowar> ZamomwioneTowary { get; set; }
    }
}
