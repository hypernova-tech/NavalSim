using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Diagnostics;

namespace ConsoleGUI
{
    public partial class MainForm : Form
    {
        UdpClient UdpClient;
        IPEndPoint EndPoint;
        public MainForm()
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
                var cmd = textBox1.Text;
                if (cmd.Contains("run")){
                    var args = cmd.Split(" ");
                    int instance_count = int.Parse(args[1]);
                    Run(instance_count);
                }
                else
                {
                    SendData(cmd);
                }
                    
                

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

        private void listBox1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            var item = listBox1.Items[listBox1.SelectedIndex];
            textBox1.Text = item.ToString();
            SendData(textBox1.Text);
        }

        private void listBox1_MouseClick(object sender, MouseEventArgs e)
        {
            var item = listBox1.Items[listBox1.SelectedIndex];
            textBox1.Text = item.ToString();
        }

        void Run(int instance_count)
        {
            for(int i = 0; i< instance_count; i++)
            {
                // Define the path to the UE4 executable
                string unrealExecutablePath = @"C:\Users\Pc\Documents\Unreal Projects\BoatSim\package\Windows\BoatSim.exe";

                // Define any command-line arguments you want to pass
                string unrealArgs = "-instance=" + i.ToString();

                // Set up the process start info
                ProcessStartInfo startInfo = new ProcessStartInfo(unrealExecutablePath, unrealArgs)
                {
                    RedirectStandardOutput = true,
                    UseShellExecute = false,
                    CreateNoWindow = true,
                };

                // Start the process
                using (Process process = new Process { StartInfo = startInfo })
                {
                    process.Start();

                    // Read the output (optional)
                    //string output = process.StandardOutput.ReadToEnd();

                    // Wait for the process to exit
                    //process.WaitForExit();
                }
            }
       
        }
    }
}