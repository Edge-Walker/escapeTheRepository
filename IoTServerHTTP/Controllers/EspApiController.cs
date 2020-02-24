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
        Array allColors = Enum.GetValues(typeof(NeoPixelColor));
        
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

            Console.Write($"Card read from {readerData.senderID}: ");
            for(int k = 0; k <= 3; k ++) {
                Console.Write($"{readerData.cardData[k]}, ");
            }

            TaggedObject obj = TaggedObject.FindObject(readerData);
            NeoPixelColor[] colors;

            if (obj != null) {
                colors = obj.colors;
            }

            else {
                colors = new NeoPixelColor[6];
                
                for(int k = 0; k < 6; k++) {
                    colors[k] = (NeoPixelColor)allColors.GetValue(random.Next(k));
                }
            }

            for (int k = 0; k < 6; ++k) {
                response += ((int)colors[k]).ToString("X6");
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
