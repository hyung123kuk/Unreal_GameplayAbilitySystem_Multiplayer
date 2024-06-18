using Microsoft.EntityFrameworkCore;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WebApi.Models;

namespace WebApi.Data
{
	public class ApplicationDbContext : DbContext
	{
		public DbSet<Member> member { get; set; }
		public DbSet<ServerList> server_list { get; set; }

		public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
			: base(options)
		{

		}
	}
}
