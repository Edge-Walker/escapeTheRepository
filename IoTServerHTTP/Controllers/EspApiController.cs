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
        Array colors = Enum.GetValues(typeof(NeoPixelColor));
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
            Console.Write($"Card was read from {readerData.sender}: ");
            for(int k = 0; k <= 3; k ++) {
                Console.Write("{readerData.cardData}[k]");
            }
            
            for(int k = 0; k < 6; k++) {
                NeoPixelColor randomColor = (NeoPixelColor)colors.GetValue(random.Next(colors.Length));
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
