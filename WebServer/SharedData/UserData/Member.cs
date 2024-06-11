using System;
using System.Collections.Generic;

namespace SharedData.UserData
{
    public partial class Member
    {
        public Member()
        {
            ChattingLog = new HashSet<ChattingLog>();
            MemberItem = new HashSet<MemberItem>();
            StorePurchaseLog = new HashSet<StorePurchaseLog>();
        }

        public string Id { get; set; }
        public string Password { get; set; }
        public DateTime CreateTime { get; set; }
        public string Introduction { get; set; } = string.Empty;
        public uint Gold { get; set; } = 0;
        public uint Exp { get; set; } = 0;

        public virtual ICollection<ChattingLog> ChattingLog { get; set; }
        public virtual ICollection<MemberItem> MemberItem { get; set; }
        public virtual ICollection<StorePurchaseLog> StorePurchaseLog { get; set; }
    }
}
