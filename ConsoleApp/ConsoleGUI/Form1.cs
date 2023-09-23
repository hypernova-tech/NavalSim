using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;

namespace ConsoleGUI
{
    public partial class Form1 : Form
    {
        UdpClient UdpClient;
        IPEndPoint EndPoint;
        public Form1()
        {
            InitializeComponent();
            InitUDPConnection();
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private void openFileDialog1_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                SendData(textBox1.Text);

            }
        }

        void InitUDPConnection()
        {


            UdpClient = new UdpClient();
            EndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 55000);

            Thread udpListenerThread = new Thread(new ThreadStart(UDPListener));
            udpListenerThread.Start();



        }

        private void UpdateListBox(string item)
        {
            if (listBox1.InvokeRequired)
            {
                listBox1.Invoke(new Action<string>(UpdateListBox), item);
            }
            else
            {
                listBox1.Items.Add(item);
            }
        }

        void UDPListener()
        {
            UdpClient client = new UdpClient();
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, 55001);
            client.Client.Bind(endPoint);
            while (true)
            {
                byte[] data = client.Receive(ref endPoint);
                string message = Encoding.ASCII.GetString(data);
                UpdateListBox(message);
            }
        }

        void SendData(string cmd)
        {
            byte[] data = Encoding.ASCII.GetBytes(cmd);
            listBox1.Items.Add(cmd);
            UdpClient.Send(data, data.Length, EndPoint);
           
        }
    }
}