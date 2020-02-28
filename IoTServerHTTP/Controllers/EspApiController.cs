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

        public EspApiController(ILogger<EspApiController> logger) {
            _logger = logger;
        }

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

        [HttpPut()]
        public IActionResult PutCardData([FromBody] CardReaderData readerData)
        {
            if (readerData == null || readerData.cardData == null || readerData.cardData.Length < 4) {
                return Error();
            }

            Console.Write($"Card read from {readerData.senderID}: ");
            for(int k = 0; k <= 3; k ++) {
                Console.Write($"{readerData.cardData[k]}, ");
            }

            TaggedObject obj = TaggedObject.FindObject(readerData);
            string response;
            if (obj == null) {
                response = "FF000000FF000000FFFF000000FF000000FF1";
            }
            else {
                response = PuzzleMaster.Instance.AssignObject(readerData.senderMacAddr, obj);
            }

            if (response == null) {
                return Error();
            }

            return Ok(response);
        }
    }
}
