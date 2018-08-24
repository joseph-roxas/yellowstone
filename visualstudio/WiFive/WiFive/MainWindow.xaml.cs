using System;
using System.IO.Ports;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Windows;
using WiFive.Models;

namespace WiFive
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            BindAvailablePorts();
        }

        private void BindAvailablePorts()
        {
            var ports = SerialPort.GetPortNames();
            ddPorts.ItemsSource = ports;
            ddPorts.SelectedIndex = 0;
        }

        private void btnDay_Click(object sender, RoutedEventArgs e)
        {
            GetTicketAsync(WifiPackage.DayPackage);
        }

        private void btnTwoDays_Click(object sender, RoutedEventArgs e)
        {
            GetTicketAsync(WifiPackage.TwoDayPackage);
        }

        private void btnThreeDays_Click(object sender, RoutedEventArgs e)
        {
            GetTicketAsync(WifiPackage.ThreeDayPackage);
        }

        private void GetTicketAsync(WifiPackage package)
        {
            var client = new HttpClient
            {
                BaseAddress = new Uri(appSettings.Default.serverUri)
            };

            client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/xml"));

            var packageName = string.Empty;
            var packageTime = string.Empty;
            switch (package)
            {
                case WifiPackage.DayPackage:
                    {
                        packageName = appSettings.Default.dayPackageName;
                        packageTime = appSettings.Default.dayPackage;
                        break;
                    }
                case WifiPackage.TwoDayPackage:
                    {
                        packageName = appSettings.Default.twoDayPackageName;
                        packageTime = appSettings.Default.twoDayPackage;
                        break;
                    }
                case WifiPackage.ThreeDayPackage:
                    {
                        packageName = appSettings.Default.threeDayPackageName;
                        packageTime = appSettings.Default.threeDayPackage;
                        break;
                    }
                case WifiPackage.WeekPackage:
                    {
                        packageName = appSettings.Default.weekPackageName;
                        packageTime = appSettings.Default.weekPackage;
                        break;
                    }
                case WifiPackage.MonthPackage:
                    {
                        packageName = appSettings.Default.monthPackageName;
                        packageTime = appSettings.Default.monthPackage;
                        break;
                    }
                default:
                    {
                        MessageBox.Show("Unknown Request.");
                        return;
                    }
            }

            var parameters = string.Format(appSettings.Default.accountCall, packageTime);

            if (MessageBox.Show(
                string.Format("Time: {0}\r\n\r\n PRINT a ticket? ", packageName),
                "Print Ticket",
                MessageBoxButton.YesNo,
                MessageBoxImage.Question) == MessageBoxResult.Yes)
            {
                try
                {
                    var response = client.GetAsync(parameters).Result;
                    if (response.IsSuccessStatusCode)
                    {
                        var jsonContent = response.Content.ReadAsStringAsync().Result.Substring(1361);
                        var accounts = XmlConvert.DeserializeObject<Accounts>(jsonContent);
                        var account = accounts.AccountList.FirstOrDefault();

                        try
                        {
                            SerialPort sp = new SerialPort
                            {
                                PortName = ddPorts.SelectedValue.ToString(),
                                BaudRate = 9600,
                                Parity = Parity.None,
                                DataBits = 8,
                                StopBits = StopBits.One
                            };
                            sp.Open();

                            var days = int.Parse(account.TimeLeft) / 86400;

                            byte[] L0 = { 0x1b, 0x40, 0x1b, 0x61, 0x01 };
                            byte[] L1 = { 0x54, 0x68, 0x61, 0x6e, 0x6b, 0x20, 0x79, 0x6f, 0x75, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x63, 0x68, 0x6f, 0x6f, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x57, 0x49, 0x46, 0x49, 0x56, 0x45 };
                            byte[] L2 = { 0x0d, 0x0a, 0x0a };
                            byte[] L3 = { 0x50, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x6c, 0x6f, 0x67, 0x69, 0x6e };
                            byte[] L4 = { 0x0d, 0x0a, 0x0a };
                            byte[] L5 = { 0x1b, 0x40, 0x1b, 0x61, 0x01, 0x1d, 0x21, 0x11 };
                            byte[] L6 = Encoding.ASCII.GetBytes(account.Username); // Ticket line
                            byte[] L7 = { 0x0d, 0x0a };
                            byte[] L8 = { 0x1d, 0x21, 0x10, 0x0a };
                            byte[] L9 = { 0x1b, 0x40, 0x1b, 0x61, 0x01 };
                            byte[] L10 = { 0x54, 0x69, 0x6d, 0x65, 0x3a, 0x20 };
                            byte[] L11 = Encoding.ASCII.GetBytes(days.ToString()); // Time
                            byte[] L12 = { 0x20, 0x64, 0x61, 0x79, 0x2f, 0x73 };
                            byte[] L13 = { 0x0d, 0x0a };
                            byte[] L14 = { 0x50, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x6b, 0x65, 0x65, 0x70, 0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x54, 0x69, 0x63, 0x6b, 0x65, 0x74 };
                            byte[] L15 = { 0x0d, 0x0a };
                            byte[] L16 = { 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a };
                            byte[] L17 = { 0x1b, 0x69 };

                            sp.Write(L0, 0, L0.Length);
                            sp.Write(L1, 0, L1.Length);
                            sp.Write(L2, 0, L2.Length);
                            sp.Write(L3, 0, L3.Length);
                            sp.Write(L4, 0, L4.Length);
                            sp.Write(L5, 0, L5.Length);
                            sp.Write(L6, 0, L6.Length);
                            sp.Write(L7, 0, L7.Length);
                            sp.Write(L8, 0, L8.Length);
                            sp.Write(L9, 0, L9.Length);
                            sp.Write(L10, 0, L10.Length);
                            sp.Write(L11, 0, L11.Length);
                            sp.Write(L12, 0, L12.Length);
                            sp.Write(L13, 0, L13.Length);
                            sp.Write(L14, 0, L14.Length);
                            sp.Write(L15, 0, L15.Length);
                            sp.Write(L16, 0, L16.Length);
                            sp.Write(L17, 0, L17.Length);
                            sp.Close();
                            sp.Dispose();
                            sp = null;
                        }
                        catch (Exception e)
                        {
                            MessageBox.Show("Printer is missing. Please contact Boss Joseph at (032) 516-2139 for more info.\r\n\r\n" + e.Message,
                                            "Printer Error",
                                            MessageBoxButton.OK,
                                            MessageBoxImage.Error);
                        }
                    }
                    else
                    {
                        MessageBox.Show("Error Code" + response.StatusCode + " : Message - " + response.ReasonPhrase);
                    }
                }
                catch
                {
                    MessageBox.Show("ANTAMEDIA is Off. Please contact Boss Joseph at (032) 516-2139 for more info.",
                                    "Server Down",
                                    MessageBoxButton.OK,
                                    MessageBoxImage.Error);
                }
            }
        }
    }
}
