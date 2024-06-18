using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

public class CheckAccountIDReq
{
    public string CheckAccountName { get; set; }
}

public class CheckAccountIDRes
{
    public bool CheckOK { get; set; }
    public string Message { get; set; }
}

public class CreateAccountPacketReq
{
    public string AccountName { get; set; }
    public string AccountPassword { get; set; }
}

public class CreateAccountPacketRes
{
    public bool CreateOK { get; set; }
    public string Message { get; set; }
}


public class LoginAccountPacketReq
{
    public string AccountName { get; set; }
    public string AccountPassword { get; set; }
}


public class LoginAccountPacketRes
{
    public bool LoginOK { get; set; }
    public string Message { get; set; }
    public List<ServerInfo> ServerList { get; set; } = new List<ServerInfo>();
}

public class ServerInfo
{ 
    public string Name { get; set; }
    public string Ip { get;set; }
    public uint CrowdedLevel { get; set; }
}




