using BotLine.Message;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace BotLine
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            // 스크롤 뷰어 항상 밑으로 설정
            SV_MessageBoxScroll.ScrollToBottom();

            // 서버 아이피 주소
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            TB_IP.Text = host.AddressList[1].ToString();

            MessageControl.Print();
        }

        private void BTN_Start_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
