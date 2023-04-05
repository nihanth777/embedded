using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
//using System.Windows.Input;

namespace Dispenser_app
{
    
    public partial class Form1 : Form
    {
        public static double counter = 0;
        //static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        public static TcpClient client = new TcpClient();
        
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            IPHostEntry ipHostInfo = System.Net.Dns.GetHostEntry("serveraddress.com");
            IPAddress ipAddress = ipHostInfo.AddressList[0];
            client.Connect(ipAddress, 21);
            client.SendTimeout = 3000;
            var status = client.Connected;
            Console.WriteLine(status);
        }
        private async void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            //Console.WriteLine("Here");
            if (char.IsWhiteSpace(e.KeyChar))
            {
                counter++;
                label1.Text = String.Format("{0:0.##}", (counter / 100));
                label3.Text = String.Format("{0:0.##}", (counter / 100) * 1.45);
                await Task.Delay(10);
            }

        }
        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            string fuel = label1.Text;
            string cost = label2.Text;
            counter = 0;
        }
        private void discharge_Click(object sender, EventArgs e)
        {
            
        }
    }
}
