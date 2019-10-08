using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace aspnetapp.Controllers
{
    public class EspApiController : Controller
    {
        string mystr = "__UNSET__";

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
            Console.Write($"Card was read from {readerData.sender}: {readerData.cardData}");
            /*-String response = "Message from server: Hello, " + 
            readerData.sender + ". Your card was: " + 
            readerData.cardData[0] + ", " + 
            readerData.cardData[1] + ", " + 
            readerData.cardData[2] + ", " + 
            readerData.cardData[3] + ", right?";
            return Ok(response);-*/

            return Ok(0x0f1466);
        }

        [HttpGet]
        public IActionResult GetString()
        {
            Console.Write("Getting string");
            return Ok(mystr);
        }
    }
}
