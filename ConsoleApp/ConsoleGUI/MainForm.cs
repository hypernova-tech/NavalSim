using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Diagnostics;
using System.ComponentModel;
using System;
using System.Collections.Generic;
using System.Xml.Linq;
using Newtonsoft.Json.Schema;
using static System.Net.Mime.MediaTypeNames;

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
        CJsonParser mJsonParser = new CJsonParser();

        public MainForm()
        {
            mJsonParser.SetMainForm(this);
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
                
                ProcessReceivedData(message);
            }
        }


        void ProcessReceivedData(string message)
        {
            UpdateListBox(message);

            if (this.InvokeRequired) // For Windows Forms replace 'this' with the name of your form if necessary
            {
                // If not, we need to invoke the UI thread and pass it a delegate of this same method
                this.Invoke(new Action<string>(TryParseJson), message);
                return;
            }

            TryParseJson(message);
        }

        void TryParseJson(string message)
        {
            mJsonParser.TryParseJson(message);
        }

       
        void SendData(string cmd, bool add_to_cmd_list = true)
        {
            for (int i = 0; i < InstanceCount; i++)
            {
                if (cmd == "")
                {
                    return;
                }
                byte[] data = Encoding.ASCII.GetBytes(cmd);
                if (add_to_cmd_list)
                {
                    AddCommandToList(cmd);
                }
                
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
        public string GetSelectedSensor()
        {
            if (SensorListBox.SelectedIndex < 0)
            {
                return "";
            }
            var selected_sensor = SensorListBox.Items[SensorListBox.SelectedIndex].ToString();

            return selected_sensor;
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
                RequestTransform(selected_sensor, false);
            }
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }

        private void TB_LocationX_TextChanged(object sender, EventArgs e)
        {

        }
        public void SetPosition(double x, double y, double z)
        {
            TB_LocationX.Text = x.ToString();
            TB_LocationY.Text = y.ToString();
            TB_LocationZ.Text = z.ToString();
        }
        public void SetRotation(double x, double y, double z)
        {
            TB_RotationX.Text = x.ToString();
            TB_RotationY.Text = y.ToString();
            TB_RotationZ.Text = z.ToString();
        }
        public void SetScale(double x, double y, double z)
        {
            TB_ScaleX.Text = x.ToString();
            TB_ScaleY.Text = y.ToString();
            TB_ScaleZ.Text = z.ToString();
        }
        void UpdatePosition()
        {
            var selected_sensor = GetSelectedSensor();
            if (selected_sensor == "")
            {
                return;
            }
            if (!double.TryParse(TB_LocationX.Text, out double valx))
            {
                return;
            }
            if (!double.TryParse(TB_LocationY.Text, out double valy))
            {
                return;
            }
            if (!double.TryParse(TB_LocationZ.Text, out double valz))
            {
                return;
            }

            string pos = "\"" + TB_LocationX.Text + " " + TB_LocationY.Text + " " + TB_LocationZ.Text + "\"";
            string command = string.Format("set --name {0} --position {1}", selected_sensor, pos);
            SendData(command);
            RequestTransform(selected_sensor, false);
        }

        void UpdateRotation()
        {
            var selected_sensor = GetSelectedSensor();
            if (selected_sensor == "")
            {
                return;
            }
            if (!double.TryParse(TB_RotationX.Text, out double valx))
            {
                return;
            }
            if (!double.TryParse(TB_RotationY.Text, out double valy))
            {
                return;
            }
            if (!double.TryParse(TB_RotationZ.Text, out double valz))
            {
                return;
            }

            string pos = "\"" + TB_RotationX.Text + " " + TB_RotationY.Text + " " + TB_RotationZ.Text + "\"";
            string command = string.Format("set --name {0} --rotation {1}", selected_sensor, pos);
            SendData(command);
            RequestTransform(selected_sensor, false);
        }

        void UpdateScale()
        {
            var selected_sensor = GetSelectedSensor();
            if (selected_sensor == "")
            {
                return;
            }
            if (!double.TryParse(TB_ScaleX.Text, out double valx))
            {
                return;
            }
            if (!double.TryParse(TB_ScaleY.Text, out double valy))
            {
                return;
            }
            if (!double.TryParse(TB_ScaleZ.Text, out double valz))
            {
                return;
            }

            string pos = "\"" + TB_ScaleX.Text + " " + TB_ScaleY.Text + " " + TB_ScaleZ.Text + "\"";
            string command = string.Format("set --name {0} --scale {1}", selected_sensor, pos);
            SendData(command);
            RequestTransform(selected_sensor, false);
        }

        private void TB_LocationX_MouseLeave(object sender, EventArgs e)
        {

        }

        private void TB_LocationX_Leave(object sender, EventArgs e)
        {

        }

        private void TB_LocationZ_TextChanged(object sender, EventArgs e)
        {

        }

        private void TB_LocationX_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdatePosition();
            }

        }

        private void TB_LocationY_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdatePosition();
            }
        }

        private void TB_LocationZ_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdatePosition();
            }
        }

        private void TB_RotationX_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdateRotation();
            }
        }

        private void TB_RotationY_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdateRotation();
            }
        }

        private void TB_RotationZ_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdateRotation();
            }
        }

        private void TB_ScaleX_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdateScale();
            }
        }

        private void TB_ScaleY_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdateScale();
            }
        }

        private void TB_ScaleZ_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab)
            {
                UpdateScale();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }

        private void timer1_Tick_1(object sender, EventArgs e)
        {
            return;
            var selected_sensor = GetSelectedSensor();
            if (selected_sensor == "")
            {
                return;
            }
            RequestTransform(selected_sensor, false);
        }

        void RequestTransform(string name, bool add_to_cmd_list)
        {
            string command = string.Format("get --name {0} --position", name);
            SendData(command);
            command = string.Format("get --name {0} --rotation", name);
            SendData(command);

            command = string.Format("get --name {0} --scale", name);
            SendData(command, false);
        }
    }
}