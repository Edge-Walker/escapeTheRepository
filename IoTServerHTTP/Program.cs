using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;

namespace IoTServerHTTP
{
    public class Program
    {
        public static void Main(string[] args)
        {
            // Force initialization of radio, etc. now
            PuzzleMaster.Instance.ToString();
            CreateWebHostBuilder(args).Build().Run();
        }

        // For some reason, the address below has to match EXACTLY the address you type into
        // the browser. For instance, if you use localhost, you CANNOT get to the server by
        // typing the external IP address of the machine into a browser.
        //
        // To expose the server to our IoT devices, you must use the IP address of the server.
        public static IWebHostBuilder CreateWebHostBuilder(string[] args) =>
            WebHost.CreateDefaultBuilder(args)
                .UseUrls("http://10.7.32.5:5001")
                .UseStartup<Startup>();
    }
}
