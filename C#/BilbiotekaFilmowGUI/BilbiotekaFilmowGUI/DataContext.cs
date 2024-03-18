using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MoviesListGUI.Models;

namespace MoviesListGUI
{
    public class DataContext : DbContext
    {
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer("");
        }

        public DbSet<User> Users { get; set; }
    }
}

//EntityFrameworkCore
//EntityFrameworkCore.SqlServer
//EntityFrameworkCore.Design