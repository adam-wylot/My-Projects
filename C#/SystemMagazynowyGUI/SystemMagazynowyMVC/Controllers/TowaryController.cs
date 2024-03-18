using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Shared;
using SystemMagazynowyMVC.Data;
using SystemMagazynowyMVC.Services;

namespace SystemMagazynowyMVC.Controllers
{
    public class TowaryController : Controller
    {
        private readonly DataContext context;
        private readonly IBasketService basketService;

        public TowaryController(DataContext context, IBasketService basketService)
        {
            this.context = context;
            this.basketService = basketService;
        }

        public async Task<IActionResult> Index()
        {
            var towary = await context.Towary.ToListAsync();
            return View(towary);
        }

        public async Task<IActionResult> AddToBasket(int id)
        {
            var towar = await context.Towary.FirstOrDefaultAsync(x => x.Id == id);
            if (towar == null)
            {
                return NotFound("Towar not found");
            }

            basketService.Add(towar);
            return RedirectToAction("Index");
        }
    }
}
