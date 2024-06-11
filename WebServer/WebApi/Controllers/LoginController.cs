using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Org.BouncyCastle.Ocsp;
using SharedData.UserData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WebApi.Data;

namespace WebApi.Controllers
{
    
    [Route("api/[controller]")]
    [ApiController]
    public class LoginController : ControllerBase
    {
        ApplicationDbContext _context;

        public LoginController(ApplicationDbContext context)
        {
            _context = context;
        }

        
        [HttpPost]
        [Route("checkId")]
        public CheckAccountIDRes CheckID([FromBody] CheckAccountIDReq req)
        {
            Console.WriteLine("아이디(" + req.CheckAccountName+ ")체크를 시도합니다.");

            CheckAccountIDRes res = new CheckAccountIDRes();
            if(req.CheckAccountName.Length < SystemValue.MinIDLen || req.CheckAccountName.Length > SystemValue.MaxIDLen)
            {
                res.CheckOK = false;
                res.Message = "아이디 길이는 "+ SystemValue.MinIDLen + "~"+ SystemValue.MaxIDLen + "자리만 가능합니다.";
                Console.WriteLine(req.CheckAccountName + "는 아이디 길이 체크에 실패합니다.");
                return res;
            }

            string PossibilityOfQueryInjectionStr = CheckThePossibilityOfQueryInjection(req.CheckAccountName);
            if (PossibilityOfQueryInjectionStr != "")
            {
                res.CheckOK = false;
                res.Message = "아이디에 " + PossibilityOfQueryInjectionStr + "는 사용할 수 없습니다.";
                Console.WriteLine(req.CheckAccountName +"는" + PossibilityOfQueryInjectionStr + "를 포함하여 아이디 체크에 실패합니다.");
                return res;
            }

            Member member = _context.member
              .AsNoTracking()
              .Where(a => a.Id == req.CheckAccountName)
              .FirstOrDefault();

            if (member == null)
            {
                res.CheckOK = true;
                res.Message = "사용가능한 ID입니다.";
                Console.WriteLine(req.CheckAccountName + "로 생성 가능합니다.");
            }
            else
            {
                res.CheckOK = false;
                res.Message = "이미 존재하는 ID입니다.";
                Console.WriteLine(req.CheckAccountName + "는 이미 존재하는 아이디로 아이디 체크에 실패합니다.");
            }

            return res;
        }

        [HttpPost]
        [Route("create")]
        public CreateAccountPacketRes CreateAccount([FromBody] CreateAccountPacketReq req)
        {
            Console.WriteLine("유저가 아이디(" + req.AccountName + ") 만들기를 시도합니다.");
            CreateAccountPacketRes res = new CreateAccountPacketRes();

            CheckAccountIDReq checkAccountIDReq = new CheckAccountIDReq();
            checkAccountIDReq.CheckAccountName = req.AccountName;
            CheckAccountIDRes checkAccountIDRes = CheckID(checkAccountIDReq);

            if(checkAccountIDRes.CheckOK == false)
            {
                res.CreateOK = false;
                res.Message = res.Message;
                Console.WriteLine(req.AccountName + "로아이디 생성에 실패합니다.");
                return res;
            }

            string SHA256Password = SHA256Hash(req.AccountPassword);
            _context.member.Add(new Member
            {
                Id = req.AccountName,
                Password = SHA256Password,
                CreateTime = System.DateTime.Now,
            });

            bool success = _context.SaveChangesEx();
            res.CreateOK = success;
            if(success == false)
            {
                res.Message = "이미 존재하는 ID입니다.";
                Console.WriteLine(req.AccountName + "는 이미 존재하는 아이디로 아이디 생성에 실패합니다.");
            }
            else
            {
                res.Message = "아이디 생성에 성공했습니다.";
            }

            Console.WriteLine(req.AccountName + "로 유저가 아이디를 생성합니다.");
            return res;
        }



        [HttpPost]
        [Route("login")]
        public LoginAccountPacketRes LoginAccount([FromBody] LoginAccountPacketReq req)
        {
            Console.WriteLine("유저가 아이디(" + req.AccountName + ")로 로그인을 시도합니다.");
            LoginAccountPacketRes res = new LoginAccountPacketRes();

            Member member = _context.member
                .AsNoTracking()
                .Where(a => a.Id == req.AccountName)
                .FirstOrDefault();

            if(member == null)
            {
                res.LoginOK = false;
                res.Message = "존재하는 ID가 없습니다.";
                Console.WriteLine(req.AccountName + "는 존재하지 않는 아이디로 로그인에 실패합니다.");
                return res;
            }

            if(!CheckPassword(member, req.AccountPassword))
            {
                res.LoginOK = false;
                res.Message = "비밀번호가 일치하지 않습니다.";
                Console.WriteLine("유저("+ req.AccountName +")는 비밀번호가 일치하지 않아 로그인에 실패합니다.");
            }

            res.LoginOK = true;
            res.ServerList = new List<ServerInfo>()
            {
                new ServerInfo {Name = "진짜 서버", Ip = "127.0.0.1", CrowdedLevel = 0 },
                new ServerInfo {Name = "진짜 서버2", Ip = "127.0.0.1", CrowdedLevel = 3 }
            };

            Console.WriteLine("유저(" + req.AccountName + ")가 로그인에 성공합니다..");
            return res;
        }

        private bool CheckPassword(Member member, string password)
        {
            string SHA256Password = SHA256Hash(password);
            if (SHA256Password == member.Password)
            {
                return true;
            }

            return false;
        }

        private string SHA256Hash(string data)
        {
            System.Security.Cryptography.SHA256 sha = new System.Security.Cryptography.SHA256Managed();
            byte[] hash = sha.ComputeHash(Encoding.ASCII.GetBytes(data));
            StringBuilder stringBuilder = new StringBuilder();
            foreach(byte b in hash)
            {
                stringBuilder.AppendFormat("{0:x2}", b);
            }

            return stringBuilder.ToString();
        }

        private string CheckThePossibilityOfQueryInjection(string Input)
        {
            string ContainedReservedWord = "";

            string[] SpecialTexts =
            { 
                "%",
                "=",
                "*",
                ">",
                ">"
            };

            string[] ReservedWords =
                {
                "OR",
                "SELECT",
                "INSERT",
                "DELETE",
                "UPDATE",
                "CREATE",
                "DROP",     
                "EXEC",
                "UNION",
                "FETCH",
                "DECLARE",
                "TRUNCATE"
            };

            foreach (string Word in SpecialTexts)
            {
                if (Input.Contains(Word))
                {
                    ContainedReservedWord = Word;
                }
            }

            foreach (string Word in ReservedWords)
            {
                if (Input.Contains(Word))
                {
                    ContainedReservedWord = Word;
                }
            }

            return ContainedReservedWord;
        }

    }
}
