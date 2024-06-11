using System;
using System.Collections.Generic;

namespace SharedData.UserData
{
    public partial class MemberItem
    {
        public uint Id { get; set; }
        public string UserId { get; set; }
        public byte ItemId { get; set; }
        public uint ItemCount { get; set; }

        public virtual StoreItem Item { get; set; }
        public virtual Member User { get; set; }
    }
}
