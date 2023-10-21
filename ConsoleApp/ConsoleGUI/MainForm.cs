using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Diagnostics;
using System.ComponentModel;
using ConsoleGUI;
using System;
using System.Collections.Generic;

namespace ConsoleGUI
{
    public partial class MainForm : Form
    {
        const string ExecutablePath = @"C:\Users\Pc\Documents\Unreal Projects\BoatSim\package\Windows\BoatSim.exe";
        UdpClient UdpClient;
        List<IPEndPoint> EndPoints = new List<IPEndPoint>();
        CCommandParser Parser = new CCommandParser();
        int CommandPointerInd;
        List<string> CommandHist = new List<string>();
        int InstanceCount = 1;
        int RemotePort = 55000;
        int LocalPort = 55050;
        HashSet<string> SensorsReceived = new HashSet<string>();
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

        void ClearCommandList()
        {
            listBox1.Items.Clear();
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {

        }

        void InitUDPConnection()
        {


            UdpClient = new UdpClient();
            for (int i = 0; i < 16; i++)
            {
                var endpoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), RemotePort + i);
                EndPoints.Add(endpoint);
            }


            Thread udpListenerThread = new Thread(new ThreadStart(UDPListener));
            udpListenerThread.Start();



        }
        private void UpdateListBoxDataThreadSafe(string item)
        {
            listBox1.Items.Add(item);
            listBox1.SelectedIndex = listBox1.Items.Count - 1;
            listBox1.ClearSelected();

            AddToSensorList(item);
        }

        private void AddToSensorList(string item)
        {
            if (item.Contains("<sensor>"))
            {
                string item_striped = item;
                item_striped = item_striped.Replace("<sensor>", "");

                if (!SensorsReceived.Contains(item_striped))
                {
                    SensorListBox.Items.Add(item_striped);
                    SensorsReceived.Add(item_striped);
                }

            }
        }

        private void UpdateListBox(string item)
        {
            if (listBox1.InvokeRequired)
            {
                listBox1.Invoke(new Action<string>(UpdateListBoxDataThreadSafe), item);

            }
            else
            {
                listBox1.Items.Add(item);
                AddToSensorList(item);
            }

        }

        void UDPListener()
        {
            UdpClient client = new UdpClient();
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, LocalPort);
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
            for (int i = 0; i < InstanceCount; i++)
            {
                if (cmd == "")
                {
                    return;
                }
                byte[] data = Encoding.ASCII.GetBytes(cmd);
                AddCommandToList(cmd);
                UdpClient.Send(data, data.Length, EndPoints[i]);
            }


        }

        void AddCommandToList(string cmd)
        {

            listBox1.SelectedIndex = listBox1.Items.Count - 1;
            listBox1.Items.Add(cmd);
            CommandHist.Add(cmd);
            CommandPointerInd = CommandHist.Count - 1;
            CommandTextBox.Text = "";
        }
        private void listBox1_MouseDoubleClick(object sender, MouseEventArgs e)
        {

        }

        private void listBox1_MouseClick(object sender, MouseEventArgs e)
        {

        }


        void Kill(int instance_no)
        {
            if (ProcessList.TryGetValue(instance_no, out var process))
            {
                try
                {
                    process.Kill();
                    process.WaitForExit();
                    process.Dispose();
                    ProcessList.Remove(instance_no);
                }
                catch (Exception ex)
                {
                }

            }


        }

        Dictionary<int, Process> ProcessList = new Dictionary<int, Process>();
        void Run(int instance_count)
        {
            InstanceCount = instance_count;
            for (int i = 0; i < instance_count; i++)
            {
                // Define the path to the UE4 executable
                string unrealExecutablePath = ExecutablePath;

                // Define any command-line arguments you want to pass
                string unrealArgs = "-windowed  -instance=" + i.ToString() + " -instancestotal=" + instance_count.ToString();

                // Set up the process start info
                ProcessStartInfo startInfo = new ProcessStartInfo(unrealExecutablePath, unrealArgs)
                {
                    RedirectStandardOutput = true,
                    UseShellExecute = false,
                    CreateNoWindow = true,

                };
                Process process = new Process();
                process.StartInfo = startInfo;
                process.Start();
                ProcessList.Add(i, process);
                // Start the process

            }

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void CommandTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                var cmd = CommandTextBox.Text;
                bool ret = Parser.TryParseCommandLine(cmd, out string outcommand, out Dictionary<string, string> outoptions, out string error_message);

                if (ret)
                {
                    if (outcommand == "process")
                    {

                        if (outoptions.TryGetValue("run", out string value))
                        {
                            Run(int.Parse(value));
                            AddCommandToList(cmd);

                        }
                        else if (outoptions.TryGetValue("kill", out string instance_to_kill))
                        {

                            SendData(cmd);
                            ProcessList.Remove(int.Parse(instance_to_kill));
                        }
                    }
                    else if (outcommand == "clear")
                    {
                        ClearCommandList();
                        CommandTextBox.Text = "";
                    }
                    else
                    {
                        SendData(cmd);
                    }


                }




            }
            else if (e.KeyCode == Keys.Up)
            {


                CommandTextBox.Text = CommandHist[CommandPointerInd];
                CommandPointerInd--;
                if (CommandPointerInd < 0)
                {
                    CommandPointerInd = 0;
                }
            }
            else if (e.KeyCode == Keys.Down)
            {


                CommandTextBox.Text = CommandHist[CommandPointerInd];
                CommandPointerInd++;
                if (CommandPointerInd >= CommandHist.Count - 1)
                {
                    CommandPointerInd = CommandHist.Count - 1;
                }
            }
        }

        private void listBox1_Click(object sender, EventArgs e)
        {

        }

        private void listBox1_MouseDoubleClick_1(object sender, MouseEventArgs e)
        {
            if (listBox1.SelectedIndex < 0)
            {
                return;
            }

            var item = listBox1.Items[listBox1.SelectedIndex];
            CommandTextBox.Text = item.ToString();
            SendData(CommandTextBox.Text);
        }

        private void listBox1_MouseClick_1(object sender, MouseEventArgs e)
        {
            if (listBox1.SelectedIndex < 0)
            {
                return;
            }
            var item = listBox1.Items[listBox1.SelectedIndex];
            CommandTextBox.Text = item.ToString();
        }

        private void SetInstanceNo_Click(object sender, EventArgs e)
        {
            if (SensorListBox.SelectedIndex < 0 || InstanceComboBox.SelectedIndex < 0)
            {
                return;
            }
            var selected_sensor = SensorListBox.Items[SensorListBox.SelectedIndex].ToString();
            var selected_text = InstanceComboBox.Items[InstanceComboBox.SelectedIndex].ToString();
            if (selected_text != "")
            {
                string command = string.Format("set --name {0} --instance {1}", selected_sensor, selected_text);
                SendData(command);
            }
        }

        private void SensorEnableCtrlButton_Click(object sender, EventArgs e)
        {
            if (SensorListBox.SelectedIndex < 0)
            {
                return;
            }
            var selected_sensor = SensorListBox.Items[SensorListBox.SelectedIndex].ToString();
            {
                string enabled = SensorEnabledCB.Checked ? "1" : "0";
                string command = string.Format("set --name {0} --enabled {1}", selected_sensor, enabled);
                SendData(command);
            }
        }

        private void SetActiveButton_Click(object sender, EventArgs e)
        {
            if (SensorListBox.SelectedIndex < 0)
            {
                return;
            }
            var selected_sensor = SensorListBox.Items[SensorListBox.SelectedIndex].ToString();
            {
                string enabled = IsActiveCB.Checked ? "1" : "0";
                string command = string.Format("set --name {0} --active {1}", selected_sensor, enabled);
                SendData(command);
            }
        }

        private void SensorListBox_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (SensorListBox.SelectedIndex < 0)
            {
                return;
            }
            var selected_sensor = SensorListBox.Items[SensorListBox.SelectedIndex].ToString();
            {
                
                string command = string.Format("set --selected {0}", selected_sensor);
                SendData(command);
            }
        }
    }
}