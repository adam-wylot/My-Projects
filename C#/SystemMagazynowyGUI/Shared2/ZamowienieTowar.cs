using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Shared
{
    public class ZamowienieTowar
    {
        public int Id { get; set; }
        public Towar Towar { get; set; }
        public Zamowienie Zamowienie { get; set; }
    }
}
