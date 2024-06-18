using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace WebApi.Models
{
    public partial class ServerList
    {
        [Key]
        public string Ip { get; set; }
        public string Name { get; set; }
        public uint CrowdedLevel { get; set; }
        public ulong IsOperation { get; set; }
    }
}
