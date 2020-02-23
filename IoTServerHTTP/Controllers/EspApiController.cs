using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace aspnetapp.Controllers
{
    public class EspApiController : Controller
    {        
        private readonly ILogger _logger;
        private String response;

        public EspApiController(ILogger<EspApiController> logger) {
            _logger = logger;
        }

        string mystr = "__UNSET__";

        // Create an array of colors so that they can be chosen
        // at random using integers.
        List<NeoPixelColor> colors = Enum.GetValues(typeof(NeoPixelColor)).Cast<NeoPixelColor>().ToList();
        
        Random random = new Random();

        

        [HttpGet]
        public IActionResult Index()
        {
            return Ok();
        }

        [HttpGet]
        public IActionResult Error()
        {
            return BadRequest();
        }

        public class Test {
            public string str { get; set; }
        }

        [HttpPut()]
        public IActionResult PutCardData([FromBody] CardReaderData readerData)
        {
            response = "";

            Console.Write($"Card was read from {readerData.senderID}: ");
            for(int k = 0; k <= 3; k ++) {
                Console.Write("{readerData.cardData}[k]");
            }

            var randomColors = colors.OrderBy(x => random.Next());

            
            for(int k = 0; k < 6; k++) {
                NeoPixelColor randomColor = (NeoPixelColor)randomColors.ElementAt(k);
                response += ((int)randomColor).ToString("X6");
            }
            
            return Ok(response);
        }

        [HttpGet]
        public IActionResult GetString()
        {
            Console.Write("Getting string");
            return Ok(mystr);
        }
    }
}
