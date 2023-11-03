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
using static System.ComponentModel.Design.ObjectSelectorEditor;
using System.Diagnostics;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using System.Reflection;

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
        HashSet<string> ActorsReceived = new HashSet<string>();
        HashSet<string> ModelsReceived = new HashSet<string>();
        CJsonParser mJsonParser = new CJsonParser();
        string Selected = "";
        List<string> MessageList = new List<string>();
        private readonly object _lockObject = new object();
        CTreeViewManager TreeViewManager = new CTreeViewManager();
        Dictionary<string, string> Modifyables = new Dictionary<string, string>();
        public MainForm()
        {
            mJsonParser.SetMainForm(this);
            InitializeComponent();
            InitUDPConnection();
            TreeViewManager.SetUpTreeViewForCustomDraw(ObjectEditor);
            LoadModels();
            LoadActorBases();
            Modifyables = CLICommandManager.GetModifiableConstants();
            PopulateDataGridView();
        }
        private void PopulateDataGridView()
        {

            ModifyableDataGrid.Columns.Add("ConstantName", "Name");
            ModifyableDataGrid.Columns.Add("ConstantValue", "Value");
            // Ensure the grid is clear before populating
            ModifyableDataGrid.Rows.Clear();

            // Get the list of modifiable constants

            // Populate the DataGridView
            foreach (var fieldName in Modifyables)
            {
                // Add a new row with the name of the constant
                int rowIndex = ModifyableDataGrid.Rows.Add();
                ModifyableDataGrid.Rows[rowIndex].Cells["ConstantName"].Value = fieldName.Key;

                // Assuming the value of the constant is a string, you can access it using reflection
                FieldInfo fieldInfo = typeof(CLICommandManager).GetField(fieldName.Key);
                if (fieldInfo != null)
                {
                    string value = (string)fieldInfo.GetValue(null); // static fields do not require an instance
                    ModifyableDataGrid.Rows[rowIndex].Cells["ConstantValue"].Value = "";
                }
            }
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

            AddToSensorList(item);
        }

        private void AddToSensorList(string item)
        {
            bool is_added = false;
            if (item.Contains("<sensor>"))
            {
                string item_striped = item;
                item_striped = item_striped.Replace("<sensor>", "");

                if (!SensorsReceived.Contains(item_striped))
                {

                    SensorsReceived.Add(item_striped);
                    is_added = true;
                    IsActorGridDirty = true;
                }

            }
            else if (item.Contains("<actor>"))
            {
                string item_striped = item;
                item_striped = item_striped.Replace("<actor>", "");

                if (!ActorsReceived.Contains(item_striped) && !SensorsReceived.Contains(item_striped))
                {

                    ActorsReceived.Add(item_striped);
                    is_added = true;
                    IsActorGridDirty = true;
                }

            }
            else if (item.Contains("<bp>"))
            {
                string item_striped = item;
                item_striped = item_striped.Replace("<bp>", "");

                if (!ModelsReceived.Contains(item_striped))
                {

                    ModelsReceived.Add(item_striped);
                    is_added = true;
                    IsActorGridDirty = true;
                }

            }
            else if (item.Contains("<clicked>"))
            {
                string item_striped = item;
                item_striped = item_striped.Replace("<clicked>", "");

                if (item_striped != "")
                {
                    Selected = item_striped;

                    SelectActor();
                }


            }


        }

        private void UpdateActorGrid()
        {
          
            int ind = 0;


            CreateSubmenu(ObjectEditorContextMenu, ModelsReceived);
#if false
            foreach (var obj in SensorsReceived)
            {
                ActorGrid.Rows.Add("Sensor", obj);
                ActorGrid.Rows[ind].HeaderCell.Value = ind.ToString();
                ActorGrid.Rows[ind].DefaultCellStyle.BackColor = Color.LightGreen;
                ind++;
            }
#endif

            foreach (var obj in ActorsReceived)
            {
#if false
                ActorGrid.Rows.Add("Actor", obj);
                ActorGrid.Rows[ind].HeaderCell.Value = ind.ToString();
                ActorGrid.Rows[ind].DefaultCellStyle.BackColor = Color.LightYellow;
                ind++;
#endif

                TreeViewManager.AddHierarchyToTreeView(ObjectEditor, obj);
            }
            SelectActor();
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

            Stopwatch stopwatch = new Stopwatch();

            List<string> mes_list = new List<string>();


            while (true)
            {
                stopwatch.Start();
                byte[] data = client.Receive(ref endPoint);
                stopwatch.Stop();
                TimeSpan elapsedTime = stopwatch.Elapsed;



                string message = Encoding.ASCII.GetString(data);
                lock (_lockObject)
                {
                    MessageList.Add(message);
                }




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
            if (!HasSelected())
            {
                return;
            }

            var selected_text = InstanceComboBox.Items[InstanceComboBox.SelectedIndex].ToString();
            if (selected_text != "")
            {
                string command = string.Format("set --name {0} --instance {1}", Selected, selected_text);
                SendData(command);
            }
        }

        private void SensorEnableCtrlButton_Click(object sender, EventArgs e)
        {
            if (!HasSelected())
            {
                return;
            }

            {
                string enabled = CBEnable.Checked ? "1" : "0";
                string command = string.Format("set --name {0} --enabled {1}", Selected, enabled);
                SendData(command);
            }
        }

        private void SetActiveButton_Click(object sender, EventArgs e)
        {
            if (!HasSelected())
            {
                return;
            }

            {
                string enabled = CBActive.Checked ? "1" : "0";
                string command = string.Format("set --name {0} --active {1}", Selected, enabled);
                SendData(command);
            }
        }
        public string GetSelectedSensor()
        {


            return Selected;
        }
        private void SensorListBox_MouseDoubleClick(object sender, MouseEventArgs e)
        {

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
            SendPosition(selected_sensor);
            RequestTransform(selected_sensor, false);
        }

        void SendPosition(string selected_sensor)
        {
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
        }

        void SendRotation(string selected_sensor)
        {
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
        }

        void SendScale(string selected_sensor)
        {
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
        }

        void UpdateRotation()
        {
            var selected_sensor = GetSelectedSensor();
            SendRotation(selected_sensor);
            RequestTransform(selected_sensor, false);
        }

        void UpdateScale()
        {
            var selected_sensor = GetSelectedSensor();
            SendScale(selected_sensor);
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
        bool IsActorGridDirty = false;
        private void timer1_Tick_1(object sender, EventArgs e)
        {
            lock (_lockObject)
            {
                IsActorGridDirty = false;
                foreach (var mes in MessageList)
                {
                    ProcessReceivedData(mes);
                }
                MessageList.Clear();

                if (IsActorGridDirty)
                {
                    UpdateActorGrid();
                }
            }
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

        private void BTN_LoadWorkspace_Click(object sender, EventArgs e)
        {

        }

        private void contextMenuStrip2_Opening(object sender, CancelEventArgs e)
        {

        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {

        }

        void LoadActors()
        {
            ActorsReceived.Clear();
            var command = ("print --actors");
            SendData(command, false);
        }

        void LoadSensors()
        {
            SensorsReceived.Clear();
            var command = ("print --sensors");
            SendData(command, false);
        }

        void LoadModels()
        {
            ModelsReceived.Clear();
            var command = ("print --bp");
            SendData(command, false);
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            LoadActors();
        }

        private void toolStripMenuItem3_Click(object sender, EventArgs e)
        {
            LoadSensors();
        }

        private void loadModelsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadModels();
        }

       

        void SelectActor()
        {

            if (Selected == "")
            {
                return;
            }
            TreeNode foundNode = FindNodeByName(ObjectEditor.Nodes, Selected);
            if (foundNode != null)
            {
                // Select the found node
                ObjectEditor.SelectedNode = foundNode;
                // Ensure that the selected node is visible to the user
                ObjectEditor.SelectedNode.EnsureVisible();
            }
            else
            {
                Selected = "";
            }
        }

        private TreeNode FindNodeByName(TreeNodeCollection nodes, string name)
        {
            foreach (TreeNode node in nodes)
            {
                // Check if the current node's name matches the search value
                if (node.Name.Equals(name, StringComparison.OrdinalIgnoreCase))
                {
                    return node; // Node with the name found
                }
                else
                {
                    // If not found, perform recursive search in child nodes
                    TreeNode foundNode = FindNodeByName(node.Nodes, name);
                    if (foundNode != null)
                    {
                        return foundNode; // Node found in a child
                    }
                }
            }
            return null; // No node found
        }




        private void createToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateFromSelected();
        }

        private void focusToolStripMenuItem_Click(object sender, EventArgs e)
        {


        }

        private bool HasSelected()
        {
            return Selected != null && Selected != "";
        }

        private void SetActiveButton_Click_1(object sender, EventArgs e)
        {

        }

        private void SensorEnableCtrlButton_Click_1(object sender, EventArgs e)
        {

        }

        private void SetInstanceNo_Click_1(object sender, EventArgs e)
        {

        }

        private void CBEnable_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox cb = (CheckBox)sender;

            if (cb != null)
            {
                if (cb.Checked)
                {
                    SendComSetComandWithSelected(CLICommandManager.Enabled, "1");
                }
                else
                {
                    SendComSetComandWithSelected(CLICommandManager.Enabled, "0");
                }

            }

        }

        void SendComSetComandWithSelected(string option, string option_value)
        {
            if (HasSelected())
            {
                string command = string.Format("set --name {0} --{1} {2}", Selected, option, option_value);
                SendData(command);
            }
            else
            {
                MessageBox.Show("select an actor");
            }
        }

        void CreateAndSendCommand(string cmd, string option1 = "", string value1 = "", string option2 = "", string value2 = "", string option3 = "", string value3 = "", string option4 = "", string value4 = "", string option5 = "", string value5 = "")
        {
            string ret = CCommandFactroy.CreateCommands(cmd, option1, value1, option2, value2, option3, value3, option4, value4, option5, value5);
            SendData(ret);
        }

        private void CBActive_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox cb = (CheckBox)sender;

            if (cb != null)
            {
                if (cb.Checked)
                {
                    SendComSetComandWithSelected(CLICommandManager.Active, "1");
                }
                else
                {
                    SendComSetComandWithSelected(CLICommandManager.Active, "0");
                }

            }
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        void CreateFromSelected(string parent = "")
        {
            if (HasSelected())
            {
                if (IsSelectedBP())
                {

                    string result = CInputDialog.Show("Enter Your Name", "Name:");
                    if (result != null)
                    {
                        // The user clicked OK and entered something.

                        {
                            CreateAndSendCommand(CLICommandManager.CreateCommand, CLICommandManager.Name, result, CLICommandManager.Bp, Selected);
                            SendPosition(result);
                            SendRotation(result);
                            SendScale(result);
                            if (parent != "")
                            {
                                Selected = result;
                                CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Name, result, CLICommandManager.Parent, parent);
                                CreateAndSendCommand(CLICommandManager.SetCommand, CLICommandManager.Selected, result);


                            }
                        }
                    }
                    else
                    {
                        // The user clicked Cancel or closed the dialog.
                        MessageBox.Show("Operation canceled.");
                    }




                }
            }
        }






        private bool IsSelectedBP()
        {
            if (Selected != "")
            {
                if (ModelsReceived.Contains(Selected))
                {
                    return true;
                }
            }

            return false;
        }



        private void destroyToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void loadAllToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        void LoadAll()
        {
            LoadModels();
            LoadSensors();
            LoadActors();

        }

        void LoadActorBases()
        {
            ActorsReceived.Clear();
            var command = ("print --actorbases");
            SendData(command, false);
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void menuStrip1_ItemClicked_1(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void loadWorkspaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (OpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                string filePath = OpenFileDialog.FileName;
                string fileName = Path.GetFileName(filePath);
                var command = string.Format("ws --load {0}", fileName);
                SendData(command, false);
            }
        }

        private void startToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var command = string.Format("sim --start");
            SendData(command, false);
        }

        private void pauseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var command = string.Format("sim --pause");
            SendData(command, false);
        }

        private void ObjectEditor_MouseClick(object sender, MouseEventArgs e)
        {

        }

        private void ObjectEditor_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node == null)
            {
                return;
            }

            string selected = e.Node.Text;

            string command = string.Format("set --selected {0}", selected);
            SendData(command);
            RequestTransform(selected, false);
            Selected = selected;
        }

        private void focusToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (HasSelected())
            {
                string command = string.Format("set --name {0} --focused", Selected);
                SendData(command);
            }
        }

        private void destroyToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            CreateAndSendCommand(CLICommandManager.DestroyCommand, CLICommandManager.Name, Selected);
            ActorsReceived.Remove(Selected);
            SensorsReceived.Remove(Selected);
            LoadActors();
            LoadSensors();
        }

        private void clearToolStripMenuItem1_Click(object sender, EventArgs e)
        {
        
            ActorsReceived.Clear();
            SensorsReceived.Clear();
            ModelsReceived.Clear();
        }

        private void loadActorsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadAll();
        }

        private void loadUserActorsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadActorBases();
        }

        public void CreateSubmenu(ContextMenuStrip contextMenuStrip, HashSet<string> items)
        {
            // First, clear the existing items if needed
            contextMenuStrip.Items.Clear();

            // Create a new ToolStripMenuItem as the main sub-menu item
            ToolStripMenuItem subMenu = new ToolStripMenuItem("Blueprints");

            // Loop through the HashSet and add each string as a ToolStripMenuItem
            foreach (string item in items)
            {
                // Create a new ToolStripMenuItem for each item
                ToolStripMenuItem menuItem = new ToolStripMenuItem(item);

                // Set the Click event handler for the sub-items here
                menuItem.Click += new EventHandler(MenuItem_Click);

                // Add the new ToolStripMenuItem to the sub-menu
                subMenu.DropDownItems.Add(menuItem);
            }

            // Add the sub-menu to the main context menu strip
            contextMenuStrip.Items.Add(subMenu);
        }

        // Event handler for sub-menu item clicks
        private void MenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem clickedItem = sender as ToolStripMenuItem;

            if (clickedItem != null)
            {
                if (clickedItem.Text != "")
                {
                    Selected = clickedItem.Text;
                    string parent = "";
                    if (ObjectEditor.SelectedNode != null)
                    {
                        parent = ObjectEditor.SelectedNode.Text;
                    }
                    CreateFromSelected(parent);
                }
            }
        }

        private void loadBlueprintsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadModels();
        }

        private void clearAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ObjectEditor.Nodes.Clear();
            ModelsReceived.Clear();
            ActorsReceived.Clear();

        }

        private void ObjectEditor_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node == null)
            {
                return;
            }

            string selected = e.Node.Text;

            string command = string.Format("set --selected {0}", selected);
            SendData(command);
            command = string.Format("set --name {0} --focused", selected);
            SendData(command);
            RequestTransform(selected, false);
            Selected = selected;
        }

        private void createToolStripMenuItem1_Click(object sender, EventArgs e)
        {

        }
    }
}
