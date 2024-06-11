using System;
using System.Collections.Generic;

namespace SharedData.UserData
{
    public partial class ChattingLog
    {
        public uint Id { get; set; }
        public string UserId { get; set; }
        public string ChatMessage { get; set; }
        public string Place { get; set; }
        public DateTime Time { get; set; }

        public virtual Member User { get; set; }
    }
}
