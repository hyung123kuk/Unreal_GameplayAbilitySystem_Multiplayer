using WebApi.Data;

namespace WebApi
{
    public static class Extensions
    {
        public static bool SaveChangesEx(this ApplicationDbContext db)
        {
            try
            {
                db.SaveChanges();
                return true;
            }
            catch 
            {
                return false; 
            }
        }
    }
}
