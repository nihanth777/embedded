using System.Diagnostics.Metrics;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Windows.Forms;
using System.Linq.Expressions;

namespace Dispenser_new
{
    public partial class Form1 : Form
    {
        private NetworkStream stream;
        private static double counter = 0;
        private static double balance = 9000;
        bool enable = true;
        TcpClient handler;
        Int32 port = 8007;
        IPAddress ipAddr = IPAddress.Parse("192.168.29.177");
        IPEndPoint ipEndPoint;
        TcpClient client = new TcpClient();
        //static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        public Form1()
        {
            InitializeComponent();
            ipEndPoint = new IPEndPoint(ipAddr, port);
            Thread t = new Thread(new ThreadStart(StartReceiving));
            t.Start();
        }
        private async void listenerAsync()
        {
            
            await client.ConnectAsync(ipEndPoint);
            NetworkStream stream = client.GetStream();
            //handler = await listener.AcceptTcpClientAsync();
        }
        private void StartReceiving()
        {
            System.Diagnostics.Debug.WriteLine("start receiving");
            Int32 port = 8009;
            IPAddress ipAddr = IPAddress.Parse("127.0.0.1");
            var endP = new IPEndPoint(ipAddr, port);
            TcpListener listener = new TcpListener(endP);
            listener.Start();
            var buffer = new byte[256];
            int i;
            try
            {
                while (true)
                {
                    handler = listener.AcceptTcpClient();
                    System.Diagnostics.Debug.WriteLine("accepted");
                    NetworkStream stream = handler.GetStream();

                    while ((i = stream.Read(buffer, 0, buffer.Length)) != 0)
                    {
                        System.Diagnostics.Debug.WriteLine("buffer !0");
                        var message = Encoding.ASCII.GetString(buffer, 0, i);
                        balance = double.Parse(message, System.Globalization.CultureInfo.InvariantCulture);
                        enable = true;
                        System.Diagnostics.Debug.WriteLine(message);
                    }
                    //int received = await stream.ReadAsync(buffer);

                }
            }catch(SocketException e)
            {
                System.Diagnostics.Debug.WriteLine(e.Message);
            }

        }
        private void connect_btn_Click(object sender, EventArgs e)
        {

        }
        private async void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!enable || balance <= (counter / 100) * 1.45) return;
            //Console.WriteLine("Here");
            if (char.IsWhiteSpace(e.KeyChar))
            {
                counter++;
                label3.Text = String.Format("{0:0.##}", (counter / 100));
                label4.Text = String.Format("{0:0.##}", (counter / 100) * 1.45);
                await Task.Delay(10);
            }

        }
        private async void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            if (!enable) return;
            string fuel = label3.Text;
            string cost = label4.Text;
            var message = fuel + "," + cost;
            await client.ConnectAsync(ipEndPoint);
            await using NetworkStream stream = client.GetStream();
            message = "get balance";
            var encodedmsg = Encoding.UTF8.GetBytes(message);
            await stream.WriteAsync(encodedmsg, 0, encodedmsg.Length);
            enable = false;
            counter = 0;
            System.Diagnostics.Debug.WriteLine("get balance");
        }
    }
}