using System;
using System.Collections.Generic;

namespace WebApi.Models
{
    public partial class StorePurchaseLog
    {
        public uint Id { get; set; }
        public string UserId { get; set; }
        public byte PurchaseItem { get; set; }
        public DateTime PurchaseTime { get; set; }

        public virtual StoreItem PurchaseItemNavigation { get; set; }
        public virtual Member User { get; set; }
    }
}
