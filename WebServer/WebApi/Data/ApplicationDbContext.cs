using Microsoft.EntityFrameworkCore;
using SharedData.UserData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WebApi.Data
{
	public class ApplicationDbContext : DbContext
	{
		public DbSet<Member> member { get; set; }

		public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
			: base(options)
		{

		}
	}
}
