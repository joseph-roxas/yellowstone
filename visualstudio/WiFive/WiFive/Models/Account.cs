using System.Collections.Generic;
using System.Xml.Serialization;

namespace WiFive.Models
{
    [XmlRoot("Accounts")]
    public class Accounts
    {
        [XmlElement("Account")]
        public List<Account> AccountList { get; set; }
    }

    public class Account
    {
        public string Username { get; set; }
        public string Password { get; set; }
        public string TimeUsed { get; set; }
        public string TimeLeft { get; set; }
        public string UnlimitedTime { get; set; }
        public string Quota { get; set; }
        public string UnlimitedQuota { get; set; }
        public string GroupPlanID { get; set; }
        public string AccessFromTo { get; set; }
        public string AccessID { get; set; }
        public string Expire { get; set; }
        public string ExpireIn { get; set; }
        public string ExpireAfter { get; set; }
        public string EnableDailyTime { get; set; }
        public string DailyTime { get; set; }
        public string EnableDailyQuota { get; set; }
        public string DailyQuota { get; set; }
        public string EnableDailyLogins { get; set; }
        public string DailyLogins { get; set; }
        public string Download { get; set; }
        public string Upload { get; set; }
        public string GenerateID { get; set; }
        public string EnableMonthlyTime { get; set; }
        public string MontlyTime { get; set; }
        public string EnableMonthyQuota { get; set; }
        public string MonthlyQuota { get; set; }
    }
}
