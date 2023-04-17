using System;
using System.IO;
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
using System.Threading;
//using System.Windows.Input;

namespace Dispenser_app
{
    
    public partial class Form1 : Form
    {
        private NetworkStream stream;
        private static double counter = 0;
        private static double balance = 0;
        bool enable = false;
        //static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        private static TcpClient client = new TcpClient();
        
        public Form1()
        {
            
            InitializeComponent();
            Thread t = new Thread(new ThreadStart(StartReceiving));
            t.Start();

        }
        private void StartReceiving()
        {
            Int32 port = 13160;
            IPAddress ipAddr = IPAddress.Parse("172.20.10.2");
            var ipEndPoint = new IPEndPoint(ipAddr, port);
            TcpClient client = new TcpClient();
            client.Connect(ipEndPoint);
            while (true)
            {
                NetworkStream stream = client.GetStream();
                if (client.ReceiveBufferSize > 0)
                {
                    var buffer = new byte[client.ReceiveBufferSize];
                    int received = stream.Read(buffer, 0, client.ReceiveBufferSize);
                    var message = Encoding.UTF8.GetString(buffer);
                    balance = double.Parse("52.8725945", System.Globalization.CultureInfo.InvariantCulture);
                    enable = true;
                }
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            
        }
        private async void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {   
            if (!enable || balance<=(counter / 100) * 1.45) return;
            //Console.WriteLine("Here");
            if (char.IsWhiteSpace(e.KeyChar))
            {
                counter++;
                label1.Text = String.Format("{0:0.##}", (counter / 100));
                label3.Text = String.Format("{0:0.##}", (counter / 100) * 1.45);
                await Task.Delay(10);
            }

        }
        private async void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            if (!enable) return;
            string fuel = label1.Text;
            string cost = label2.Text;
            var message = fuel+","+cost;
            Int32 port = 8009;
            IPAddress ipAddr = IPAddress.Parse("127.0.0.1");
            var ipEndPoint = new IPEndPoint(ipAddr, port);
            TcpListener listener = new TcpListener(ipEndPoint);
            listener.Start();
            TcpClient handler = await listener.AcceptTcpClientAsync();
            NetworkStream stream = handler.GetStream();
            var encodedmsg = Encoding.UTF8.GetBytes(message);
            await stream.WriteAsync(encodedmsg, 0, encodedmsg.Length);
            enable = false;
            counter = 0;
        }
        private void discharge_Click(object sender, EventArgs e)
        {
            
        }
    }
}
