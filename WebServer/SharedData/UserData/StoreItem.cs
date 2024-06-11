using System;
using System.Collections.Generic;

namespace SharedData.UserData
{
    public partial class StoreItem
    {
        public StoreItem()
        {
            MemberItem = new HashSet<MemberItem>();
            StorePurchaseLog = new HashSet<StorePurchaseLog>();
        }

        public byte Id { get; set; }
        public string Name { get; set; }
        public int Cost { get; set; }
        public string Image { get; set; }
        public string Explanation { get; set; }
        public ulong IsSale { get; set; }

        public virtual ICollection<MemberItem> MemberItem { get; set; }
        public virtual ICollection<StorePurchaseLog> StorePurchaseLog { get; set; }
    }
}
