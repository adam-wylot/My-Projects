using Microsoft.AspNetCore.Mvc;
using SystemMagazynowyMVC.Services;

namespace SystemMagazynowyMVC.Controllers
{
    public class OrderController : Controller
    {
        private readonly IBasketService basketService;

        public OrderController(IBasketService basketService)
        {
            this.basketService = basketService;
        }

        public IActionResult Index()
        {
            return View(basketService.Towary.OrderBy(x => x.Nazwa).ToList());
        }

        public IActionResult RemoveFromBasket(int id)
        {
            var towar = basketService.Towary.FirstOrDefault(x => x.Id == id);
            if (towar == null)
            {
                return NotFound("Towar not found");
            }

            basketService.Remove(towar);
            return RedirectToAction("Index");
        }
    }
}
