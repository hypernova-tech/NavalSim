using System.Xml.Linq;

namespace ConsoleGUI
{
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            TreeNode treeNode1 = new TreeNode("World");
            DataGridViewCellStyle dataGridViewCellStyle1 = new DataGridViewCellStyle();
            DataGridViewCellStyle dataGridViewCellStyle2 = new DataGridViewCellStyle();
            DataGridViewCellStyle dataGridViewCellStyle3 = new DataGridViewCellStyle();
            OpenFileDialog = new OpenFileDialog();
            contextMenuStrip1 = new ContextMenuStrip(components);
            ActorListContextMenu = new ContextMenuStrip(components);
            createToolStripMenuItem = new ToolStripMenuItem();
            CommandTextBox = new TextBox();
            listBox1 = new ListBox();
            contextMenuConsoleListBox = new ContextMenuStrip(components);
            clearCommandsToolStripMenuItem = new ToolStripMenuItem();
            SensorTimer = new System.Windows.Forms.Timer(components);
            tabPage2 = new TabPage();
            ObjectEditor = new TreeView();
            ObjectEditorContextMenu = new ContextMenuStrip(components);
            blueprintToolStripMenuItem = new ToolStripMenuItem();
            attachToPathToolStripMenuItem = new ToolStripMenuItem();
            detachPathToolStripMenuItem = new ToolStripMenuItem();
            destroyToolStripMenuItem = new ToolStripMenuItem();
            flowLayoutPanel1 = new FlowLayoutPanel();
            label10 = new Label();
            TBName = new TextBox();
            groupBox4 = new GroupBox();
            InstanceComboBox = new ComboBox();
            SetInstanceNo = new Button();
            CBActive = new CheckBox();
            CBEnable = new CheckBox();
            groupBox1 = new GroupBox();
            label3 = new Label();
            label2 = new Label();
            label1 = new Label();
            TB_LocationZ = new TextBox();
            TB_LocationY = new TextBox();
            TB_LocationX = new TextBox();
            groupBox2 = new GroupBox();
            label4 = new Label();
            label5 = new Label();
            label6 = new Label();
            TB_RotationZ = new TextBox();
            TB_RotationY = new TextBox();
            TB_RotationX = new TextBox();
            groupBox3 = new GroupBox();
            label7 = new Label();
            label8 = new Label();
            label9 = new Label();
            TB_ScaleZ = new TextBox();
            TB_ScaleY = new TextBox();
            TB_ScaleX = new TextBox();
            ModifyableDataGrid = new DataGridView();
            tabControl1 = new TabControl();
            tabPage1 = new TabPage();
            tabPage3 = new TabPage();
            TbPathSegmentCount = new TextBox();
            label15 = new Label();
            TbTurnRate = new TextBox();
            label14 = new Label();
            TbPathSpeed = new TextBox();
            label13 = new Label();
            CBIsCurved = new CheckBox();
            CBIsLooped = new CheckBox();
            label12 = new Label();
            TbPathName = new TextBox();
            label11 = new Label();
            button1 = new Button();
            WaypointGrid = new DataGridView();
            Column2 = new DataGridViewTextBoxColumn();
            Column3 = new DataGridViewTextBoxColumn();
            Column4 = new DataGridViewTextBoxColumn();
            PathContextMenu = new ContextMenuStrip(components);
            addToolStripMenuItem = new ToolStripMenuItem();
            removeToolStripMenuItem = new ToolStripMenuItem();
            tabPage4 = new TabPage();
            label24 = new Label();
            TBWindAzimuth = new TrackBar();
            label23 = new Label();
            TBWind = new TrackBar();
            LBTimeValue = new Label();
            label22 = new Label();
            TBTimeOfDay = new TrackBar();
            label21 = new Label();
            TBFog = new TrackBar();
            label20 = new Label();
            TBDust = new TrackBar();
            label19 = new Label();
            BThunder = new TrackBar();
            label18 = new Label();
            TBSnow = new TrackBar();
            label17 = new Label();
            TBRain = new TrackBar();
            label16 = new Label();
            TBCloud = new TrackBar();
            tabPage5 = new TabPage();
            TBTTopLeftH = new TextBox();
            TBTTopLeftLon = new TextBox();
            TBTTopLeftLat = new TextBox();
            TBTBottomRightH = new TextBox();
            TBTBottomRightLon = new TextBox();
            TBTBottomRightLat = new TextBox();
            label30 = new Label();
            TBDMapMaxLevel = new TextBox();
            label34 = new Label();
            TBDMapMinLevel = new TextBox();
            label35 = new Label();
            TBTerrainName = new TextBox();
            label26 = new Label();
            BTCreate = new Button();
            label33 = new Label();
            TBHMapMaxLevel = new TextBox();
            label32 = new Label();
            TBHMapMinLevel = new TextBox();
            label31 = new Label();
            TBDMapTexture = new TextBox();
            label27 = new Label();
            TBImaryTexture = new TextBox();
            LBImaryTexture = new Label();
            TBHMapName = new TextBox();
            label25 = new Label();
            menuStrip1 = new MenuStrip();
            fileToolStripMenuItem = new ToolStripMenuItem();
            loadWorkspaceToolStripMenuItem = new ToolStripMenuItem();
            saveWorkspaceToolStripMenuItem = new ToolStripMenuItem();
            toolStripMenuItem4 = new ToolStripMenuItem();
            loadUserActorsToolStripMenuItem = new ToolStripMenuItem();
            toolStripMenuItem2 = new ToolStripMenuItem();
            loadActorsToolStripMenuItem = new ToolStripMenuItem();
            viewToolStripMenuItem = new ToolStripMenuItem();
            clearAllToolStripMenuItem = new ToolStripMenuItem();
            showChartsToolStripMenuItem = new ToolStripMenuItem();
            hideChartsToolStripMenuItem = new ToolStripMenuItem();
            cameaToolStripMenuItem = new ToolStripMenuItem();
            viewToolStripMenuItem1 = new ToolStripMenuItem();
            topToolStripMenuItem1 = new ToolStripMenuItem();
            leftToolStripMenuItem1 = new ToolStripMenuItem();
            rightToolStripMenuItem1 = new ToolStripMenuItem();
            frontToolStripMenuItem = new ToolStripMenuItem();
            backToolStripMenuItem = new ToolStripMenuItem();
            speedToolStripMenuItem = new ToolStripMenuItem();
            xToolStripMenuItem = new ToolStripMenuItem();
            xToolStripMenuItem1 = new ToolStripMenuItem();
            xToolStripMenuItem2 = new ToolStripMenuItem();
            xToolStripMenuItem3 = new ToolStripMenuItem();
            xToolStripMenuItem4 = new ToolStripMenuItem();
            xToolStripMenuItem5 = new ToolStripMenuItem();
            xToolStripMenuItem6 = new ToolStripMenuItem();
            xToolStripMenuItem7 = new ToolStripMenuItem();
            xToolStripMenuItem8 = new ToolStripMenuItem();
            simulationToolStripMenuItem = new ToolStripMenuItem();
            startToolStripMenuItem = new ToolStripMenuItem();
            pauseToolStripMenuItem = new ToolStripMenuItem();
            resumeToolStripMenuItem = new ToolStripMenuItem();
            contextObjectEditor = new ContextMenuStrip(components);
            focusToolStripMenuItem1 = new ToolStripMenuItem();
            destroyToolStripMenuItem1 = new ToolStripMenuItem();
            clearToolStripMenuItem1 = new ToolStripMenuItem();
            ConsoeListBoxContextMenu = new ContextMenuStrip(components);
            clearToolStripMenuItem = new ToolStripMenuItem();
            FileSaver = new SaveFileDialog();
            ActorListContextMenu.SuspendLayout();
            contextMenuConsoleListBox.SuspendLayout();
            tabPage2.SuspendLayout();
            ObjectEditorContextMenu.SuspendLayout();
            flowLayoutPanel1.SuspendLayout();
            groupBox4.SuspendLayout();
            groupBox1.SuspendLayout();
            groupBox2.SuspendLayout();
            groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)ModifyableDataGrid).BeginInit();
            tabControl1.SuspendLayout();
            tabPage1.SuspendLayout();
            tabPage3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)WaypointGrid).BeginInit();
            PathContextMenu.SuspendLayout();
            tabPage4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)TBWindAzimuth).BeginInit();
            ((System.ComponentModel.ISupportInitialize)TBWind).BeginInit();
            ((System.ComponentModel.ISupportInitialize)TBTimeOfDay).BeginInit();
            ((System.ComponentModel.ISupportInitialize)TBFog).BeginInit();
            ((System.ComponentModel.ISupportInitialize)TBDust).BeginInit();
            ((System.ComponentModel.ISupportInitialize)BThunder).BeginInit();
            ((System.ComponentModel.ISupportInitialize)TBSnow).BeginInit();
            ((System.ComponentModel.ISupportInitialize)TBRain).BeginInit();
            ((System.ComponentModel.ISupportInitialize)TBCloud).BeginInit();
            tabPage5.SuspendLayout();
            menuStrip1.SuspendLayout();
            contextObjectEditor.SuspendLayout();
            ConsoeListBoxContextMenu.SuspendLayout();
            SuspendLayout();
            // 
            // OpenFileDialog
            // 
            OpenFileDialog.FileName = "openFileDialog1";
            OpenFileDialog.FileOk += openFileDialog1_FileOk;
            // 
            // contextMenuStrip1
            // 
            contextMenuStrip1.Name = "contextMenuStrip1";
            contextMenuStrip1.Size = new Size(61, 4);
            // 
            // ActorListContextMenu
            // 
            ActorListContextMenu.Items.AddRange(new ToolStripItem[] { createToolStripMenuItem });
            ActorListContextMenu.Name = "ActorListContextMenu";
            ActorListContextMenu.Size = new Size(177, 26);
            ActorListContextMenu.Opening += contextMenuStrip2_Opening;
            // 
            // createToolStripMenuItem
            // 
            createToolStripMenuItem.Name = "createToolStripMenuItem";
            createToolStripMenuItem.Size = new Size(176, 22);
            createToolStripMenuItem.Text = "Create From Model";
            createToolStripMenuItem.Click += createToolStripMenuItem_Click;
            // 
            // CommandTextBox
            // 
            CommandTextBox.Location = new Point(15, 498);
            CommandTextBox.Name = "CommandTextBox";
            CommandTextBox.Size = new Size(483, 23);
            CommandTextBox.TabIndex = 4;
            CommandTextBox.KeyDown += CommandTextBox_KeyDown;
            // 
            // listBox1
            // 
            listBox1.ContextMenuStrip = contextMenuConsoleListBox;
            listBox1.FormattingEnabled = true;
            listBox1.HorizontalScrollbar = true;
            listBox1.ItemHeight = 15;
            listBox1.Location = new Point(15, 23);
            listBox1.Name = "listBox1";
            listBox1.Size = new Size(483, 469);
            listBox1.TabIndex = 3;
            listBox1.Click += listBox1_Click;
            listBox1.MouseClick += listBox1_MouseClick_1;
            listBox1.MouseDoubleClick += listBox1_MouseDoubleClick_1;
            // 
            // contextMenuConsoleListBox
            // 
            contextMenuConsoleListBox.Items.AddRange(new ToolStripItem[] { clearCommandsToolStripMenuItem });
            contextMenuConsoleListBox.Name = "contextMenuConsoleListBox";
            contextMenuConsoleListBox.Size = new Size(167, 26);
            // 
            // clearCommandsToolStripMenuItem
            // 
            clearCommandsToolStripMenuItem.Name = "clearCommandsToolStripMenuItem";
            clearCommandsToolStripMenuItem.Size = new Size(166, 22);
            clearCommandsToolStripMenuItem.Text = "Clear Commands";
            clearCommandsToolStripMenuItem.Click += clearCommandsToolStripMenuItem_Click;
            // 
            // SensorTimer
            // 
            SensorTimer.Enabled = true;
            SensorTimer.Interval = 75;
            SensorTimer.Tick += timer1_Tick_1;
            // 
            // tabPage2
            // 
            tabPage2.Controls.Add(ObjectEditor);
            tabPage2.Controls.Add(flowLayoutPanel1);
            tabPage2.Location = new Point(4, 24);
            tabPage2.Name = "tabPage2";
            tabPage2.Padding = new Padding(3);
            tabPage2.Size = new Size(548, 650);
            tabPage2.TabIndex = 1;
            tabPage2.Text = "Actors";
            tabPage2.UseVisualStyleBackColor = true;
            // 
            // ObjectEditor
            // 
            ObjectEditor.ContextMenuStrip = ObjectEditorContextMenu;
            ObjectEditor.Location = new Point(13, 11);
            ObjectEditor.Name = "ObjectEditor";
            treeNode1.Name = "World";
            treeNode1.Text = "World";
            ObjectEditor.Nodes.AddRange(new TreeNode[] { treeNode1 });
            ObjectEditor.Size = new Size(246, 633);
            ObjectEditor.TabIndex = 18;
            ObjectEditor.NodeMouseClick += ObjectEditor_NodeMouseClick;
            ObjectEditor.NodeMouseDoubleClick += ObjectEditor_NodeMouseDoubleClick;
            ObjectEditor.MouseClick += ObjectEditor_MouseClick;
            // 
            // ObjectEditorContextMenu
            // 
            ObjectEditorContextMenu.Items.AddRange(new ToolStripItem[] { blueprintToolStripMenuItem, attachToPathToolStripMenuItem, detachPathToolStripMenuItem, destroyToolStripMenuItem });
            ObjectEditorContextMenu.Name = "contextMenuStrip2";
            ObjectEditorContextMenu.Size = new Size(139, 92);
            ObjectEditorContextMenu.Opening += ObjectEditorContextMenu_Opening;
            // 
            // blueprintToolStripMenuItem
            // 
            blueprintToolStripMenuItem.Name = "blueprintToolStripMenuItem";
            blueprintToolStripMenuItem.Size = new Size(138, 22);
            blueprintToolStripMenuItem.Text = "Blueprints";
            // 
            // attachToPathToolStripMenuItem
            // 
            attachToPathToolStripMenuItem.Name = "attachToPathToolStripMenuItem";
            attachToPathToolStripMenuItem.Size = new Size(138, 22);
            attachToPathToolStripMenuItem.Text = "Attach Path";
            // 
            // detachPathToolStripMenuItem
            // 
            detachPathToolStripMenuItem.Name = "detachPathToolStripMenuItem";
            detachPathToolStripMenuItem.Size = new Size(138, 22);
            detachPathToolStripMenuItem.Text = "Detach Path";
            // 
            // destroyToolStripMenuItem
            // 
            destroyToolStripMenuItem.Name = "destroyToolStripMenuItem";
            destroyToolStripMenuItem.Size = new Size(138, 22);
            destroyToolStripMenuItem.Text = "Destroy";
            destroyToolStripMenuItem.Click += destroyToolStripMenuItem_Click_1;
            // 
            // flowLayoutPanel1
            // 
            flowLayoutPanel1.AutoScroll = true;
            flowLayoutPanel1.Controls.Add(label10);
            flowLayoutPanel1.Controls.Add(TBName);
            flowLayoutPanel1.Controls.Add(groupBox4);
            flowLayoutPanel1.Controls.Add(groupBox1);
            flowLayoutPanel1.Controls.Add(groupBox2);
            flowLayoutPanel1.Controls.Add(groupBox3);
            flowLayoutPanel1.Controls.Add(ModifyableDataGrid);
            flowLayoutPanel1.Location = new Point(265, 11);
            flowLayoutPanel1.Name = "flowLayoutPanel1";
            flowLayoutPanel1.Size = new Size(277, 633);
            flowLayoutPanel1.TabIndex = 17;
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Location = new Point(3, 7);
            label10.Margin = new Padding(3, 7, 3, 0);
            label10.Name = "label10";
            label10.Size = new Size(39, 15);
            label10.TabIndex = 31;
            label10.Text = "Name";
            // 
            // TBName
            // 
            TBName.Location = new Point(48, 3);
            TBName.Name = "TBName";
            TBName.Size = new Size(85, 23);
            TBName.TabIndex = 30;
            // 
            // groupBox4
            // 
            groupBox4.Controls.Add(InstanceComboBox);
            groupBox4.Controls.Add(SetInstanceNo);
            groupBox4.Controls.Add(CBActive);
            groupBox4.Controls.Add(CBEnable);
            groupBox4.Location = new Point(3, 32);
            groupBox4.Name = "groupBox4";
            groupBox4.Size = new Size(253, 149);
            groupBox4.TabIndex = 30;
            groupBox4.TabStop = false;
            groupBox4.Text = "Control";
            // 
            // InstanceComboBox
            // 
            InstanceComboBox.FormattingEnabled = true;
            InstanceComboBox.Items.AddRange(new object[] { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15" });
            InstanceComboBox.Location = new Point(16, 21);
            InstanceComboBox.Name = "InstanceComboBox";
            InstanceComboBox.Size = new Size(108, 23);
            InstanceComboBox.TabIndex = 21;
            // 
            // SetInstanceNo
            // 
            SetInstanceNo.Location = new Point(16, 50);
            SetInstanceNo.Name = "SetInstanceNo";
            SetInstanceNo.Size = new Size(97, 23);
            SetInstanceNo.TabIndex = 22;
            SetInstanceNo.Text = "Set Instance";
            SetInstanceNo.UseVisualStyleBackColor = true;
            SetInstanceNo.Click += SetInstanceNo_Click_1;
            // 
            // CBActive
            // 
            CBActive.AutoSize = true;
            CBActive.Location = new Point(16, 104);
            CBActive.Name = "CBActive";
            CBActive.Size = new Size(59, 19);
            CBActive.TabIndex = 25;
            CBActive.Text = "Active";
            CBActive.UseVisualStyleBackColor = true;
            CBActive.CheckedChanged += CBActive_CheckedChanged;
            // 
            // CBEnable
            // 
            CBEnable.AutoSize = true;
            CBEnable.Location = new Point(16, 79);
            CBEnable.Name = "CBEnable";
            CBEnable.Size = new Size(61, 19);
            CBEnable.TabIndex = 23;
            CBEnable.Text = "Enable";
            CBEnable.UseVisualStyleBackColor = true;
            CBEnable.CheckedChanged += CBEnable_CheckedChanged;
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(label3);
            groupBox1.Controls.Add(label2);
            groupBox1.Controls.Add(label1);
            groupBox1.Controls.Add(TB_LocationZ);
            groupBox1.Controls.Add(TB_LocationY);
            groupBox1.Controls.Add(TB_LocationX);
            groupBox1.Location = new Point(3, 187);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(253, 111);
            groupBox1.TabIndex = 27;
            groupBox1.TabStop = false;
            groupBox1.Text = "Location";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(4, 83);
            label3.Name = "label3";
            label3.Size = new Size(14, 15);
            label3.TabIndex = 12;
            label3.Text = "Z";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(4, 54);
            label2.Name = "label2";
            label2.Size = new Size(14, 15);
            label2.TabIndex = 11;
            label2.Text = "Y";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(4, 26);
            label1.Name = "label1";
            label1.Size = new Size(14, 15);
            label1.TabIndex = 10;
            label1.Text = "X";
            // 
            // TB_LocationZ
            // 
            TB_LocationZ.Location = new Point(24, 80);
            TB_LocationZ.Name = "TB_LocationZ";
            TB_LocationZ.Size = new Size(223, 23);
            TB_LocationZ.TabIndex = 9;
            TB_LocationZ.Text = "0";
            TB_LocationZ.KeyDown += TB_LocationZ_KeyDown;
            // 
            // TB_LocationY
            // 
            TB_LocationY.Location = new Point(24, 51);
            TB_LocationY.Name = "TB_LocationY";
            TB_LocationY.Size = new Size(223, 23);
            TB_LocationY.TabIndex = 8;
            TB_LocationY.Text = "0";
            TB_LocationY.KeyDown += TB_LocationY_KeyDown;
            // 
            // TB_LocationX
            // 
            TB_LocationX.Location = new Point(24, 23);
            TB_LocationX.Name = "TB_LocationX";
            TB_LocationX.Size = new Size(223, 23);
            TB_LocationX.TabIndex = 7;
            TB_LocationX.Text = "0";
            TB_LocationX.KeyDown += TB_LocationX_KeyDown;
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(label4);
            groupBox2.Controls.Add(label5);
            groupBox2.Controls.Add(label6);
            groupBox2.Controls.Add(TB_RotationZ);
            groupBox2.Controls.Add(TB_RotationY);
            groupBox2.Controls.Add(TB_RotationX);
            groupBox2.Location = new Point(3, 304);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(253, 111);
            groupBox2.TabIndex = 28;
            groupBox2.TabStop = false;
            groupBox2.Text = "Rotation";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(4, 83);
            label4.Name = "label4";
            label4.Size = new Size(14, 15);
            label4.TabIndex = 12;
            label4.Text = "Z";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(4, 54);
            label5.Name = "label5";
            label5.Size = new Size(14, 15);
            label5.TabIndex = 11;
            label5.Text = "Y";
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new Point(4, 26);
            label6.Name = "label6";
            label6.Size = new Size(14, 15);
            label6.TabIndex = 10;
            label6.Text = "X";
            // 
            // TB_RotationZ
            // 
            TB_RotationZ.Location = new Point(24, 80);
            TB_RotationZ.Name = "TB_RotationZ";
            TB_RotationZ.Size = new Size(223, 23);
            TB_RotationZ.TabIndex = 9;
            TB_RotationZ.Text = "0";
            TB_RotationZ.KeyDown += TB_RotationZ_KeyDown;
            // 
            // TB_RotationY
            // 
            TB_RotationY.Location = new Point(24, 51);
            TB_RotationY.Name = "TB_RotationY";
            TB_RotationY.Size = new Size(223, 23);
            TB_RotationY.TabIndex = 8;
            TB_RotationY.Text = "0";
            TB_RotationY.KeyDown += TB_RotationY_KeyDown;
            // 
            // TB_RotationX
            // 
            TB_RotationX.Location = new Point(24, 23);
            TB_RotationX.Name = "TB_RotationX";
            TB_RotationX.Size = new Size(223, 23);
            TB_RotationX.TabIndex = 7;
            TB_RotationX.Text = "0";
            TB_RotationX.KeyDown += TB_RotationX_KeyDown;
            // 
            // groupBox3
            // 
            groupBox3.Controls.Add(label7);
            groupBox3.Controls.Add(label8);
            groupBox3.Controls.Add(label9);
            groupBox3.Controls.Add(TB_ScaleZ);
            groupBox3.Controls.Add(TB_ScaleY);
            groupBox3.Controls.Add(TB_ScaleX);
            groupBox3.Location = new Point(3, 421);
            groupBox3.Name = "groupBox3";
            groupBox3.Size = new Size(253, 107);
            groupBox3.TabIndex = 29;
            groupBox3.TabStop = false;
            groupBox3.Text = "Scale";
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new Point(4, 83);
            label7.Name = "label7";
            label7.Size = new Size(14, 15);
            label7.TabIndex = 12;
            label7.Text = "Z";
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Location = new Point(4, 54);
            label8.Name = "label8";
            label8.Size = new Size(14, 15);
            label8.TabIndex = 11;
            label8.Text = "Y";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Location = new Point(4, 26);
            label9.Name = "label9";
            label9.Size = new Size(14, 15);
            label9.TabIndex = 10;
            label9.Text = "X";
            // 
            // TB_ScaleZ
            // 
            TB_ScaleZ.Location = new Point(24, 80);
            TB_ScaleZ.Name = "TB_ScaleZ";
            TB_ScaleZ.Size = new Size(223, 23);
            TB_ScaleZ.TabIndex = 9;
            TB_ScaleZ.Text = "1";
            TB_ScaleZ.KeyDown += TB_ScaleZ_KeyDown;
            // 
            // TB_ScaleY
            // 
            TB_ScaleY.Location = new Point(24, 51);
            TB_ScaleY.Name = "TB_ScaleY";
            TB_ScaleY.Size = new Size(223, 23);
            TB_ScaleY.TabIndex = 8;
            TB_ScaleY.Text = "1";
            TB_ScaleY.KeyDown += TB_ScaleY_KeyDown;
            // 
            // TB_ScaleX
            // 
            TB_ScaleX.Location = new Point(24, 23);
            TB_ScaleX.Name = "TB_ScaleX";
            TB_ScaleX.Size = new Size(223, 23);
            TB_ScaleX.TabIndex = 7;
            TB_ScaleX.Text = "1";
            TB_ScaleX.KeyDown += TB_ScaleX_KeyDown;
            // 
            // ModifyableDataGrid
            // 
            dataGridViewCellStyle1.Alignment = DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = SystemColors.Control;
            dataGridViewCellStyle1.Font = new Font("Segoe UI", 1F, FontStyle.Regular, GraphicsUnit.Point);
            dataGridViewCellStyle1.ForeColor = SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = DataGridViewTriState.True;
            ModifyableDataGrid.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            ModifyableDataGrid.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle2.Alignment = DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = SystemColors.Window;
            dataGridViewCellStyle2.Font = new Font("Segoe UI", 8F, FontStyle.Regular, GraphicsUnit.Point);
            dataGridViewCellStyle2.ForeColor = SystemColors.ControlText;
            dataGridViewCellStyle2.SelectionBackColor = SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = DataGridViewTriState.False;
            ModifyableDataGrid.DefaultCellStyle = dataGridViewCellStyle2;
            ModifyableDataGrid.Location = new Point(3, 534);
            ModifyableDataGrid.Name = "ModifyableDataGrid";
            dataGridViewCellStyle3.Alignment = DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = SystemColors.Control;
            dataGridViewCellStyle3.Font = new Font("Segoe UI", 6F, FontStyle.Regular, GraphicsUnit.Point);
            dataGridViewCellStyle3.ForeColor = SystemColors.WindowText;
            dataGridViewCellStyle3.SelectionBackColor = SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = DataGridViewTriState.True;
            ModifyableDataGrid.RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
            ModifyableDataGrid.RowHeadersVisible = false;
            ModifyableDataGrid.RowTemplate.Height = 25;
            ModifyableDataGrid.Size = new Size(253, 405);
            ModifyableDataGrid.TabIndex = 34;
            ModifyableDataGrid.CellContentClick += ModifyableDataGrid_CellContentClick;
            ModifyableDataGrid.CellEndEdit += ModifyableDataGrid_CellEndEdit;
            ModifyableDataGrid.CellEnter += ModifyableDataGrid_CellEnter;
            ModifyableDataGrid.CellValueChanged += ModifyableDataGrid_CellValueChanged;
            ModifyableDataGrid.EditingControlShowing += ModifyableDataGrid_EditingControlShowing;
            ModifyableDataGrid.KeyDown += ModifyableDataGrid_KeyDown;
            ModifyableDataGrid.KeyPress += ModifyableDataGrid_KeyPress;
            // 
            // tabControl1
            // 
            tabControl1.Controls.Add(tabPage2);
            tabControl1.Controls.Add(tabPage1);
            tabControl1.Controls.Add(tabPage3);
            tabControl1.Controls.Add(tabPage4);
            tabControl1.Controls.Add(tabPage5);
            tabControl1.Location = new Point(6, 27);
            tabControl1.Name = "tabControl1";
            tabControl1.SelectedIndex = 0;
            tabControl1.Size = new Size(556, 678);
            tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            tabPage1.Controls.Add(listBox1);
            tabPage1.Controls.Add(CommandTextBox);
            tabPage1.Location = new Point(4, 24);
            tabPage1.Name = "tabPage1";
            tabPage1.Padding = new Padding(3);
            tabPage1.Size = new Size(548, 650);
            tabPage1.TabIndex = 2;
            tabPage1.Text = "Console";
            tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            tabPage3.Controls.Add(TbPathSegmentCount);
            tabPage3.Controls.Add(label15);
            tabPage3.Controls.Add(TbTurnRate);
            tabPage3.Controls.Add(label14);
            tabPage3.Controls.Add(TbPathSpeed);
            tabPage3.Controls.Add(label13);
            tabPage3.Controls.Add(CBIsCurved);
            tabPage3.Controls.Add(CBIsLooped);
            tabPage3.Controls.Add(label12);
            tabPage3.Controls.Add(TbPathName);
            tabPage3.Controls.Add(label11);
            tabPage3.Controls.Add(button1);
            tabPage3.Controls.Add(WaypointGrid);
            tabPage3.Location = new Point(4, 24);
            tabPage3.Name = "tabPage3";
            tabPage3.Size = new Size(548, 650);
            tabPage3.TabIndex = 3;
            tabPage3.Text = "Path";
            tabPage3.UseVisualStyleBackColor = true;
            // 
            // TbPathSegmentCount
            // 
            TbPathSegmentCount.Location = new Point(106, 35);
            TbPathSegmentCount.Name = "TbPathSegmentCount";
            TbPathSegmentCount.Size = new Size(100, 23);
            TbPathSegmentCount.TabIndex = 12;
            TbPathSegmentCount.Text = "50";
            // 
            // label15
            // 
            label15.AutoSize = true;
            label15.Location = new Point(4, 38);
            label15.Name = "label15";
            label15.Size = new Size(90, 15);
            label15.TabIndex = 11;
            label15.Text = "Segment Count";
            // 
            // TbTurnRate
            // 
            TbTurnRate.Location = new Point(106, 129);
            TbTurnRate.Name = "TbTurnRate";
            TbTurnRate.Size = new Size(100, 23);
            TbTurnRate.TabIndex = 10;
            TbTurnRate.Text = "50";
            // 
            // label14
            // 
            label14.AutoSize = true;
            label14.Location = new Point(4, 132);
            label14.Name = "label14";
            label14.Size = new Size(96, 15);
            label14.TabIndex = 9;
            label14.Text = "Turn Rate(Deg/s)";
            // 
            // TbPathSpeed
            // 
            TbPathSpeed.Location = new Point(106, 100);
            TbPathSpeed.Name = "TbPathSpeed";
            TbPathSpeed.Size = new Size(100, 23);
            TbPathSpeed.TabIndex = 8;
            TbPathSpeed.Text = "10";
            // 
            // label13
            // 
            label13.AutoSize = true;
            label13.Location = new Point(4, 103);
            label13.Name = "label13";
            label13.Size = new Size(68, 15);
            label13.TabIndex = 7;
            label13.Text = "Speed(m/s)";
            // 
            // CBIsCurved
            // 
            CBIsCurved.AutoSize = true;
            CBIsCurved.Checked = true;
            CBIsCurved.CheckState = CheckState.Checked;
            CBIsCurved.Location = new Point(4, 81);
            CBIsCurved.Name = "CBIsCurved";
            CBIsCurved.Size = new Size(64, 19);
            CBIsCurved.TabIndex = 6;
            CBIsCurved.Text = "Curved";
            CBIsCurved.UseVisualStyleBackColor = true;
            // 
            // CBIsLooped
            // 
            CBIsLooped.AutoSize = true;
            CBIsLooped.Checked = true;
            CBIsLooped.CheckState = CheckState.Checked;
            CBIsLooped.Location = new Point(4, 56);
            CBIsLooped.Name = "CBIsLooped";
            CBIsLooped.Size = new Size(53, 19);
            CBIsLooped.TabIndex = 5;
            CBIsLooped.Text = "Loop";
            CBIsLooped.UseVisualStyleBackColor = true;
            // 
            // label12
            // 
            label12.AutoSize = true;
            label12.Location = new Point(7, 43);
            label12.Name = "label12";
            label12.Size = new Size(0, 15);
            label12.TabIndex = 4;
            // 
            // TbPathName
            // 
            TbPathName.Location = new Point(106, 10);
            TbPathName.Name = "TbPathName";
            TbPathName.Size = new Size(100, 23);
            TbPathName.TabIndex = 3;
            TbPathName.Text = "path1";
            // 
            // label11
            // 
            label11.AutoSize = true;
            label11.Location = new Point(7, 13);
            label11.Name = "label11";
            label11.Size = new Size(39, 15);
            label11.TabIndex = 2;
            label11.Text = "Name";
            // 
            // button1
            // 
            button1.Location = new Point(0, 483);
            button1.Name = "button1";
            button1.Size = new Size(124, 27);
            button1.TabIndex = 1;
            button1.Text = "Create Path";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // WaypointGrid
            // 
            WaypointGrid.AllowUserToAddRows = false;
            WaypointGrid.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            WaypointGrid.Columns.AddRange(new DataGridViewColumn[] { Column2, Column3, Column4 });
            WaypointGrid.ContextMenuStrip = PathContextMenu;
            WaypointGrid.Location = new Point(2, 212);
            WaypointGrid.MultiSelect = false;
            WaypointGrid.Name = "WaypointGrid";
            WaypointGrid.RowTemplate.Height = 25;
            WaypointGrid.Size = new Size(344, 265);
            WaypointGrid.TabIndex = 0;
            // 
            // Column2
            // 
            Column2.HeaderText = "X";
            Column2.Name = "Column2";
            // 
            // Column3
            // 
            Column3.HeaderText = "Y";
            Column3.Name = "Column3";
            // 
            // Column4
            // 
            Column4.HeaderText = "Z";
            Column4.Name = "Column4";
            // 
            // PathContextMenu
            // 
            PathContextMenu.Items.AddRange(new ToolStripItem[] { addToolStripMenuItem, removeToolStripMenuItem });
            PathContextMenu.Name = "PathContextMenu";
            PathContextMenu.Size = new Size(118, 48);
            // 
            // addToolStripMenuItem
            // 
            addToolStripMenuItem.Name = "addToolStripMenuItem";
            addToolStripMenuItem.Size = new Size(117, 22);
            addToolStripMenuItem.Text = "Add";
            addToolStripMenuItem.Click += addToolStripMenuItem_Click;
            // 
            // removeToolStripMenuItem
            // 
            removeToolStripMenuItem.Name = "removeToolStripMenuItem";
            removeToolStripMenuItem.Size = new Size(117, 22);
            removeToolStripMenuItem.Text = "Remove";
            removeToolStripMenuItem.Click += removeToolStripMenuItem_Click;
            // 
            // tabPage4
            // 
            tabPage4.Controls.Add(label24);
            tabPage4.Controls.Add(TBWindAzimuth);
            tabPage4.Controls.Add(label23);
            tabPage4.Controls.Add(TBWind);
            tabPage4.Controls.Add(LBTimeValue);
            tabPage4.Controls.Add(label22);
            tabPage4.Controls.Add(TBTimeOfDay);
            tabPage4.Controls.Add(label21);
            tabPage4.Controls.Add(TBFog);
            tabPage4.Controls.Add(label20);
            tabPage4.Controls.Add(TBDust);
            tabPage4.Controls.Add(label19);
            tabPage4.Controls.Add(BThunder);
            tabPage4.Controls.Add(label18);
            tabPage4.Controls.Add(TBSnow);
            tabPage4.Controls.Add(label17);
            tabPage4.Controls.Add(TBRain);
            tabPage4.Controls.Add(label16);
            tabPage4.Controls.Add(TBCloud);
            tabPage4.Location = new Point(4, 24);
            tabPage4.Name = "tabPage4";
            tabPage4.Size = new Size(548, 650);
            tabPage4.TabIndex = 4;
            tabPage4.Text = "Environment";
            tabPage4.UseVisualStyleBackColor = true;
            // 
            // label24
            // 
            label24.AutoSize = true;
            label24.Location = new Point(17, 526);
            label24.Name = "label24";
            label24.Size = new Size(83, 15);
            label24.TabIndex = 18;
            label24.Text = "Wind Azimuth";
            // 
            // TBWindAzimuth
            // 
            TBWindAzimuth.Location = new Point(97, 512);
            TBWindAzimuth.Margin = new Padding(3, 3, 3, 0);
            TBWindAzimuth.Maximum = 100;
            TBWindAzimuth.Name = "TBWindAzimuth";
            TBWindAzimuth.Size = new Size(330, 45);
            TBWindAzimuth.TabIndex = 17;
            TBWindAzimuth.ValueChanged += TBWindAzimuth_ValueChanged;
            // 
            // label23
            // 
            label23.AutoSize = true;
            label23.Location = new Point(17, 460);
            label23.Name = "label23";
            label23.Size = new Size(35, 15);
            label23.TabIndex = 16;
            label23.Text = "Wind";
            // 
            // TBWind
            // 
            TBWind.Location = new Point(97, 446);
            TBWind.Margin = new Padding(3, 3, 3, 0);
            TBWind.Maximum = 100;
            TBWind.Name = "TBWind";
            TBWind.Size = new Size(330, 45);
            TBWind.TabIndex = 15;
            TBWind.ValueChanged += TBWind_ValueChanged;
            // 
            // LBTimeValue
            // 
            LBTimeValue.AutoSize = true;
            LBTimeValue.Location = new Point(218, 51);
            LBTimeValue.Name = "LBTimeValue";
            LBTimeValue.Size = new Size(0, 15);
            LBTimeValue.TabIndex = 14;
            // 
            // label22
            // 
            label22.AutoSize = true;
            label22.Location = new Point(17, 35);
            label22.Name = "label22";
            label22.Size = new Size(52, 15);
            label22.TabIndex = 13;
            label22.Text = "Time(hr)";
            // 
            // TBTimeOfDay
            // 
            TBTimeOfDay.Location = new Point(97, 21);
            TBTimeOfDay.Margin = new Padding(3, 3, 3, 0);
            TBTimeOfDay.Maximum = 100;
            TBTimeOfDay.Name = "TBTimeOfDay";
            TBTimeOfDay.Size = new Size(330, 45);
            TBTimeOfDay.TabIndex = 12;
            TBTimeOfDay.Value = 50;
            TBTimeOfDay.Scroll += TBTimeOfDay_Scroll;
            TBTimeOfDay.ValueChanged += TBTimeOfDay_ValueChanged;
            // 
            // label21
            // 
            label21.AutoSize = true;
            label21.Location = new Point(17, 397);
            label21.Name = "label21";
            label21.Size = new Size(27, 15);
            label21.TabIndex = 11;
            label21.Text = "Fog";
            // 
            // TBFog
            // 
            TBFog.Location = new Point(97, 383);
            TBFog.Margin = new Padding(3, 3, 3, 0);
            TBFog.Maximum = 100;
            TBFog.Name = "TBFog";
            TBFog.Size = new Size(330, 45);
            TBFog.TabIndex = 10;
            TBFog.ValueChanged += TBFog_ValueChanged;
            // 
            // label20
            // 
            label20.AutoSize = true;
            label20.Location = new Point(17, 336);
            label20.Name = "label20";
            label20.Size = new Size(31, 15);
            label20.TabIndex = 9;
            label20.Text = "Dust";
            // 
            // TBDust
            // 
            TBDust.Location = new Point(97, 322);
            TBDust.Margin = new Padding(3, 3, 3, 0);
            TBDust.Maximum = 100;
            TBDust.Name = "TBDust";
            TBDust.Size = new Size(330, 45);
            TBDust.TabIndex = 8;
            TBDust.ValueChanged += TBDust_ValueChanged;
            // 
            // label19
            // 
            label19.AutoSize = true;
            label19.Location = new Point(17, 277);
            label19.Name = "label19";
            label19.Size = new Size(51, 15);
            label19.TabIndex = 7;
            label19.Text = "Thunder";
            // 
            // BThunder
            // 
            BThunder.Location = new Point(97, 263);
            BThunder.Margin = new Padding(3, 3, 3, 0);
            BThunder.Maximum = 100;
            BThunder.Name = "BThunder";
            BThunder.Size = new Size(330, 45);
            BThunder.TabIndex = 6;
            BThunder.ValueChanged += BThunder_ValueChanged;
            // 
            // label18
            // 
            label18.AutoSize = true;
            label18.Location = new Point(17, 217);
            label18.Name = "label18";
            label18.Size = new Size(36, 15);
            label18.TabIndex = 5;
            label18.Text = "Snow";
            // 
            // TBSnow
            // 
            TBSnow.Location = new Point(97, 203);
            TBSnow.Margin = new Padding(3, 3, 3, 0);
            TBSnow.Maximum = 100;
            TBSnow.Name = "TBSnow";
            TBSnow.Size = new Size(330, 45);
            TBSnow.TabIndex = 4;
            TBSnow.ValueChanged += TBSnow_ValueChanged;
            // 
            // label17
            // 
            label17.AutoSize = true;
            label17.Location = new Point(17, 157);
            label17.Name = "label17";
            label17.Size = new Size(30, 15);
            label17.TabIndex = 3;
            label17.Text = "Rain";
            // 
            // TBRain
            // 
            TBRain.Location = new Point(97, 143);
            TBRain.Margin = new Padding(3, 3, 3, 0);
            TBRain.Maximum = 100;
            TBRain.Name = "TBRain";
            TBRain.Size = new Size(330, 45);
            TBRain.TabIndex = 2;
            TBRain.ValueChanged += TBRain_ValueChanged;
            // 
            // label16
            // 
            label16.AutoSize = true;
            label16.Location = new Point(17, 98);
            label16.Name = "label16";
            label16.Size = new Size(39, 15);
            label16.TabIndex = 1;
            label16.Text = "Cloud";
            // 
            // TBCloud
            // 
            TBCloud.Location = new Point(97, 84);
            TBCloud.Margin = new Padding(3, 3, 3, 0);
            TBCloud.Maximum = 100;
            TBCloud.Name = "TBCloud";
            TBCloud.Size = new Size(330, 45);
            TBCloud.TabIndex = 0;
            TBCloud.ValueChanged += TBCloud_ValueChanged;
            // 
            // tabPage5
            // 
            tabPage5.Controls.Add(TBTTopLeftH);
            tabPage5.Controls.Add(TBTTopLeftLon);
            tabPage5.Controls.Add(TBTTopLeftLat);
            tabPage5.Controls.Add(TBTBottomRightH);
            tabPage5.Controls.Add(TBTBottomRightLon);
            tabPage5.Controls.Add(TBTBottomRightLat);
            tabPage5.Controls.Add(label30);
            tabPage5.Controls.Add(TBDMapMaxLevel);
            tabPage5.Controls.Add(label34);
            tabPage5.Controls.Add(TBDMapMinLevel);
            tabPage5.Controls.Add(label35);
            tabPage5.Controls.Add(TBTerrainName);
            tabPage5.Controls.Add(label26);
            tabPage5.Controls.Add(BTCreate);
            tabPage5.Controls.Add(label33);
            tabPage5.Controls.Add(TBHMapMaxLevel);
            tabPage5.Controls.Add(label32);
            tabPage5.Controls.Add(TBHMapMinLevel);
            tabPage5.Controls.Add(label31);
            tabPage5.Controls.Add(TBDMapTexture);
            tabPage5.Controls.Add(label27);
            tabPage5.Controls.Add(TBImaryTexture);
            tabPage5.Controls.Add(LBImaryTexture);
            tabPage5.Controls.Add(TBHMapName);
            tabPage5.Controls.Add(label25);
            tabPage5.Location = new Point(4, 24);
            tabPage5.Name = "tabPage5";
            tabPage5.Size = new Size(548, 650);
            tabPage5.TabIndex = 5;
            tabPage5.Text = "Terrain";
            tabPage5.UseVisualStyleBackColor = true;
            // 
            // TBTTopLeftH
            // 
            TBTTopLeftH.Location = new Point(314, 256);
            TBTTopLeftH.Name = "TBTTopLeftH";
            TBTTopLeftH.Size = new Size(72, 23);
            TBTTopLeftH.TabIndex = 38;
            TBTTopLeftH.Text = "0";
            TBTTopLeftH.TextChanged += TBTTopLeftH_TextChanged;
            // 
            // TBTTopLeftLon
            // 
            TBTTopLeftLon.Location = new Point(236, 256);
            TBTTopLeftLon.Name = "TBTTopLeftLon";
            TBTTopLeftLon.Size = new Size(72, 23);
            TBTTopLeftLon.TabIndex = 37;
            TBTTopLeftLon.Text = "29.445356290";
            TBTTopLeftLon.TextChanged += TBTTopLeftLon_TextChanged;
            // 
            // TBTTopLeftLat
            // 
            TBTTopLeftLat.Location = new Point(158, 256);
            TBTTopLeftLat.Name = "TBTTopLeftLat";
            TBTTopLeftLat.Size = new Size(72, 23);
            TBTTopLeftLat.TabIndex = 36;
            TBTTopLeftLat.Text = "40.82096524";
            TBTTopLeftLat.TextChanged += TBTTopLeftLat_TextChanged;
            // 
            // TBTBottomRightH
            // 
            TBTBottomRightH.Location = new Point(314, 285);
            TBTBottomRightH.Name = "TBTBottomRightH";
            TBTBottomRightH.Size = new Size(72, 23);
            TBTBottomRightH.TabIndex = 35;
            TBTBottomRightH.Text = "0";
            // 
            // TBTBottomRightLon
            // 
            TBTBottomRightLon.Location = new Point(236, 285);
            TBTBottomRightLon.Name = "TBTBottomRightLon";
            TBTBottomRightLon.Size = new Size(72, 23);
            TBTBottomRightLon.TabIndex = 34;
            TBTBottomRightLon.Text = "29.65047026";
            // 
            // TBTBottomRightLat
            // 
            TBTBottomRightLat.Location = new Point(158, 285);
            TBTBottomRightLat.Name = "TBTBottomRightLat";
            TBTBottomRightLat.Size = new Size(72, 23);
            TBTBottomRightLat.TabIndex = 33;
            TBTBottomRightLat.Text = "40.6655628";
            // 
            // label30
            // 
            label30.AutoSize = true;
            label30.Location = new Point(15, 288);
            label30.Name = "label30";
            label30.Size = new Size(116, 15);
            label30.TabIndex = 32;
            label30.Text = "Bottom Right(lt,ln,h)";
            // 
            // TBDMapMaxLevel
            // 
            TBDMapMaxLevel.Location = new Point(159, 225);
            TBDMapMaxLevel.Name = "TBDMapMaxLevel";
            TBDMapMaxLevel.Size = new Size(56, 23);
            TBDMapMaxLevel.TabIndex = 31;
            TBDMapMaxLevel.Text = "-100";
            // 
            // label34
            // 
            label34.AutoSize = true;
            label34.Location = new Point(16, 228);
            label34.Name = "label34";
            label34.Size = new Size(132, 15);
            label34.TabIndex = 30;
            label34.Text = "DepthMapMaxLevel(m)";
            // 
            // TBDMapMinLevel
            // 
            TBDMapMinLevel.Location = new Point(159, 196);
            TBDMapMinLevel.Name = "TBDMapMinLevel";
            TBDMapMinLevel.Size = new Size(56, 23);
            TBDMapMinLevel.TabIndex = 29;
            TBDMapMinLevel.Text = "-30";
            // 
            // label35
            // 
            label35.AutoSize = true;
            label35.Location = new Point(16, 199);
            label35.Name = "label35";
            label35.Size = new Size(130, 15);
            label35.TabIndex = 28;
            label35.Text = "DepthMapMinLevel(m)";
            // 
            // TBTerrainName
            // 
            TBTerrainName.Location = new Point(159, 22);
            TBTerrainName.Name = "TBTerrainName";
            TBTerrainName.Size = new Size(312, 23);
            TBTerrainName.TabIndex = 27;
            TBTerrainName.Text = "terrain1";
            // 
            // label26
            // 
            label26.AutoSize = true;
            label26.Location = new Point(16, 25);
            label26.Name = "label26";
            label26.Size = new Size(77, 15);
            label26.TabIndex = 26;
            label26.Text = "Terrain Name";
            // 
            // BTCreate
            // 
            BTCreate.Location = new Point(179, 323);
            BTCreate.Name = "BTCreate";
            BTCreate.Size = new Size(114, 34);
            BTCreate.TabIndex = 25;
            BTCreate.Text = "Create Terrain";
            BTCreate.UseVisualStyleBackColor = true;
            BTCreate.Click += BTCreate_Click;
            // 
            // label33
            // 
            label33.AutoSize = true;
            label33.Location = new Point(15, 259);
            label33.Name = "label33";
            label33.Size = new Size(87, 15);
            label33.TabIndex = 20;
            label33.Text = "Top Left(lt,ln,h)";
            // 
            // TBHMapMaxLevel
            // 
            TBHMapMaxLevel.Location = new Point(159, 167);
            TBHMapMaxLevel.Name = "TBHMapMaxLevel";
            TBHMapMaxLevel.Size = new Size(56, 23);
            TBHMapMaxLevel.TabIndex = 19;
            TBHMapMaxLevel.Text = "500";
            // 
            // label32
            // 
            label32.AutoSize = true;
            label32.Location = new Point(16, 170);
            label32.Name = "label32";
            label32.Size = new Size(136, 15);
            label32.TabIndex = 18;
            label32.Text = "HeightMapMaxLevel(m)";
            // 
            // TBHMapMinLevel
            // 
            TBHMapMinLevel.Location = new Point(159, 138);
            TBHMapMinLevel.Name = "TBHMapMinLevel";
            TBHMapMinLevel.Size = new Size(56, 23);
            TBHMapMinLevel.TabIndex = 17;
            TBHMapMinLevel.Text = "0";
            // 
            // label31
            // 
            label31.AutoSize = true;
            label31.Location = new Point(16, 141);
            label31.Name = "label31";
            label31.Size = new Size(134, 15);
            label31.TabIndex = 16;
            label31.Text = "HeightMapMinLevel(m)";
            // 
            // TBDMapTexture
            // 
            TBDMapTexture.Location = new Point(159, 109);
            TBDMapTexture.Name = "TBDMapTexture";
            TBDMapTexture.Size = new Size(312, 23);
            TBDMapTexture.TabIndex = 9;
            TBDMapTexture.Text = "dmap.png";
            // 
            // label27
            // 
            label27.AutoSize = true;
            label27.Location = new Point(16, 112);
            label27.Name = "label27";
            label27.Size = new Size(66, 15);
            label27.TabIndex = 8;
            label27.Text = "Depth Map";
            // 
            // TBImaryTexture
            // 
            TBImaryTexture.Location = new Point(159, 80);
            TBImaryTexture.Name = "TBImaryTexture";
            TBImaryTexture.Size = new Size(312, 23);
            TBImaryTexture.TabIndex = 7;
            TBImaryTexture.Text = "hmaptexturesat.png";
            // 
            // LBImaryTexture
            // 
            LBImaryTexture.AutoSize = true;
            LBImaryTexture.Location = new Point(16, 83);
            LBImaryTexture.Name = "LBImaryTexture";
            LBImaryTexture.Size = new Size(91, 15);
            LBImaryTexture.TabIndex = 6;
            LBImaryTexture.Text = "Imagery Texture";
            // 
            // TBHMapName
            // 
            TBHMapName.Location = new Point(159, 51);
            TBHMapName.Name = "TBHMapName";
            TBHMapName.Size = new Size(312, 23);
            TBHMapName.TabIndex = 5;
            TBHMapName.Text = "hmap.png";
            // 
            // label25
            // 
            label25.AutoSize = true;
            label25.Location = new Point(16, 54);
            label25.Name = "label25";
            label25.Size = new Size(67, 15);
            label25.TabIndex = 4;
            label25.Text = "HeightMap";
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, viewToolStripMenuItem, cameaToolStripMenuItem, simulationToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(562, 24);
            menuStrip1.TabIndex = 5;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { loadWorkspaceToolStripMenuItem, saveWorkspaceToolStripMenuItem, toolStripMenuItem4, loadUserActorsToolStripMenuItem, toolStripMenuItem2, loadActorsToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new Size(37, 20);
            fileToolStripMenuItem.Text = "File";
            // 
            // loadWorkspaceToolStripMenuItem
            // 
            loadWorkspaceToolStripMenuItem.Name = "loadWorkspaceToolStripMenuItem";
            loadWorkspaceToolStripMenuItem.Size = new Size(163, 22);
            loadWorkspaceToolStripMenuItem.Text = "Load Workspace";
            loadWorkspaceToolStripMenuItem.Click += loadWorkspaceToolStripMenuItem_Click;
            // 
            // saveWorkspaceToolStripMenuItem
            // 
            saveWorkspaceToolStripMenuItem.Name = "saveWorkspaceToolStripMenuItem";
            saveWorkspaceToolStripMenuItem.Size = new Size(163, 22);
            saveWorkspaceToolStripMenuItem.Text = "Save Workspace";
            saveWorkspaceToolStripMenuItem.Click += saveWorkspaceToolStripMenuItem_Click;
            // 
            // toolStripMenuItem4
            // 
            toolStripMenuItem4.Name = "toolStripMenuItem4";
            toolStripMenuItem4.Size = new Size(163, 22);
            toolStripMenuItem4.Text = "  ";
            // 
            // loadUserActorsToolStripMenuItem
            // 
            loadUserActorsToolStripMenuItem.Name = "loadUserActorsToolStripMenuItem";
            loadUserActorsToolStripMenuItem.Size = new Size(163, 22);
            loadUserActorsToolStripMenuItem.Text = "Load User Actors";
            loadUserActorsToolStripMenuItem.Click += loadUserActorsToolStripMenuItem_Click;
            // 
            // toolStripMenuItem2
            // 
            toolStripMenuItem2.Name = "toolStripMenuItem2";
            toolStripMenuItem2.Size = new Size(163, 22);
            toolStripMenuItem2.Text = "  ";
            // 
            // loadActorsToolStripMenuItem
            // 
            loadActorsToolStripMenuItem.Name = "loadActorsToolStripMenuItem";
            loadActorsToolStripMenuItem.Size = new Size(163, 22);
            loadActorsToolStripMenuItem.Text = "Load Scene";
            loadActorsToolStripMenuItem.Click += loadActorsToolStripMenuItem_Click;
            // 
            // viewToolStripMenuItem
            // 
            viewToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { clearAllToolStripMenuItem, showChartsToolStripMenuItem, hideChartsToolStripMenuItem });
            viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            viewToolStripMenuItem.Size = new Size(44, 20);
            viewToolStripMenuItem.Text = "View";
            // 
            // clearAllToolStripMenuItem
            // 
            clearAllToolStripMenuItem.Name = "clearAllToolStripMenuItem";
            clearAllToolStripMenuItem.Size = new Size(140, 22);
            clearAllToolStripMenuItem.Text = "Clear All";
            clearAllToolStripMenuItem.Click += clearAllToolStripMenuItem_Click;
            // 
            // showChartsToolStripMenuItem
            // 
            showChartsToolStripMenuItem.Name = "showChartsToolStripMenuItem";
            showChartsToolStripMenuItem.Size = new Size(140, 22);
            showChartsToolStripMenuItem.Text = "Show Charts";
            showChartsToolStripMenuItem.Click += showChartsToolStripMenuItem_Click;
            // 
            // hideChartsToolStripMenuItem
            // 
            hideChartsToolStripMenuItem.Name = "hideChartsToolStripMenuItem";
            hideChartsToolStripMenuItem.Size = new Size(140, 22);
            hideChartsToolStripMenuItem.Text = "Hide Charts";
            hideChartsToolStripMenuItem.Click += hideChartsToolStripMenuItem_Click;
            // 
            // cameaToolStripMenuItem
            // 
            cameaToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { viewToolStripMenuItem1, speedToolStripMenuItem });
            cameaToolStripMenuItem.Name = "cameaToolStripMenuItem";
            cameaToolStripMenuItem.Size = new Size(60, 20);
            cameaToolStripMenuItem.Text = "Camera";
            // 
            // viewToolStripMenuItem1
            // 
            viewToolStripMenuItem1.DropDownItems.AddRange(new ToolStripItem[] { topToolStripMenuItem1, leftToolStripMenuItem1, rightToolStripMenuItem1, frontToolStripMenuItem, backToolStripMenuItem });
            viewToolStripMenuItem1.Name = "viewToolStripMenuItem1";
            viewToolStripMenuItem1.Size = new Size(106, 22);
            viewToolStripMenuItem1.Text = "View";
            // 
            // topToolStripMenuItem1
            // 
            topToolStripMenuItem1.Name = "topToolStripMenuItem1";
            topToolStripMenuItem1.Size = new Size(102, 22);
            topToolStripMenuItem1.Text = "Top";
            topToolStripMenuItem1.Click += topToolStripMenuItem1_Click;
            // 
            // leftToolStripMenuItem1
            // 
            leftToolStripMenuItem1.Name = "leftToolStripMenuItem1";
            leftToolStripMenuItem1.Size = new Size(102, 22);
            leftToolStripMenuItem1.Text = "Left";
            leftToolStripMenuItem1.Click += leftToolStripMenuItem1_Click;
            // 
            // rightToolStripMenuItem1
            // 
            rightToolStripMenuItem1.Name = "rightToolStripMenuItem1";
            rightToolStripMenuItem1.Size = new Size(102, 22);
            rightToolStripMenuItem1.Text = "Right";
            rightToolStripMenuItem1.Click += rightToolStripMenuItem1_Click;
            // 
            // frontToolStripMenuItem
            // 
            frontToolStripMenuItem.Name = "frontToolStripMenuItem";
            frontToolStripMenuItem.Size = new Size(102, 22);
            frontToolStripMenuItem.Text = "Front";
            frontToolStripMenuItem.Click += frontToolStripMenuItem_Click;
            // 
            // backToolStripMenuItem
            // 
            backToolStripMenuItem.Name = "backToolStripMenuItem";
            backToolStripMenuItem.Size = new Size(102, 22);
            backToolStripMenuItem.Text = "Back";
            backToolStripMenuItem.Click += backToolStripMenuItem_Click;
            // 
            // speedToolStripMenuItem
            // 
            speedToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { xToolStripMenuItem, xToolStripMenuItem1, xToolStripMenuItem2, xToolStripMenuItem3, xToolStripMenuItem4, xToolStripMenuItem5, xToolStripMenuItem6, xToolStripMenuItem7, xToolStripMenuItem8 });
            speedToolStripMenuItem.Name = "speedToolStripMenuItem";
            speedToolStripMenuItem.Size = new Size(106, 22);
            speedToolStripMenuItem.Text = "Speed";
            speedToolStripMenuItem.Click += speedToolStripMenuItem_Click;
            // 
            // xToolStripMenuItem
            // 
            xToolStripMenuItem.Name = "xToolStripMenuItem";
            xToolStripMenuItem.Size = new Size(95, 22);
            xToolStripMenuItem.Text = "1x";
            xToolStripMenuItem.Click += xToolStripMenuItem_Click;
            // 
            // xToolStripMenuItem1
            // 
            xToolStripMenuItem1.Name = "xToolStripMenuItem1";
            xToolStripMenuItem1.Size = new Size(95, 22);
            xToolStripMenuItem1.Text = "1.5x";
            xToolStripMenuItem1.Click += xToolStripMenuItem1_Click;
            // 
            // xToolStripMenuItem2
            // 
            xToolStripMenuItem2.Name = "xToolStripMenuItem2";
            xToolStripMenuItem2.Size = new Size(95, 22);
            xToolStripMenuItem2.Text = "2x";
            xToolStripMenuItem2.Click += xToolStripMenuItem2_Click;
            // 
            // xToolStripMenuItem3
            // 
            xToolStripMenuItem3.Name = "xToolStripMenuItem3";
            xToolStripMenuItem3.Size = new Size(95, 22);
            xToolStripMenuItem3.Text = "3x";
            xToolStripMenuItem3.Click += xToolStripMenuItem3_Click;
            // 
            // xToolStripMenuItem4
            // 
            xToolStripMenuItem4.Name = "xToolStripMenuItem4";
            xToolStripMenuItem4.Size = new Size(95, 22);
            xToolStripMenuItem4.Text = "5x";
            xToolStripMenuItem4.Click += xToolStripMenuItem4_Click;
            // 
            // xToolStripMenuItem5
            // 
            xToolStripMenuItem5.Name = "xToolStripMenuItem5";
            xToolStripMenuItem5.Size = new Size(95, 22);
            xToolStripMenuItem5.Text = "10x";
            xToolStripMenuItem5.Click += xToolStripMenuItem5_Click;
            // 
            // xToolStripMenuItem6
            // 
            xToolStripMenuItem6.Name = "xToolStripMenuItem6";
            xToolStripMenuItem6.Size = new Size(95, 22);
            xToolStripMenuItem6.Text = "15x";
            xToolStripMenuItem6.Click += xToolStripMenuItem6_Click;
            // 
            // xToolStripMenuItem7
            // 
            xToolStripMenuItem7.Name = "xToolStripMenuItem7";
            xToolStripMenuItem7.Size = new Size(95, 22);
            xToolStripMenuItem7.Text = "25x";
            xToolStripMenuItem7.Click += xToolStripMenuItem7_Click;
            // 
            // xToolStripMenuItem8
            // 
            xToolStripMenuItem8.Name = "xToolStripMenuItem8";
            xToolStripMenuItem8.Size = new Size(95, 22);
            xToolStripMenuItem8.Text = "50x";
            xToolStripMenuItem8.Click += xToolStripMenuItem8_Click;
            // 
            // simulationToolStripMenuItem
            // 
            simulationToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { startToolStripMenuItem, pauseToolStripMenuItem, resumeToolStripMenuItem });
            simulationToolStripMenuItem.Name = "simulationToolStripMenuItem";
            simulationToolStripMenuItem.Size = new Size(76, 20);
            simulationToolStripMenuItem.Text = "Simulation";
            // 
            // startToolStripMenuItem
            // 
            startToolStripMenuItem.Name = "startToolStripMenuItem";
            startToolStripMenuItem.Size = new Size(116, 22);
            startToolStripMenuItem.Text = "Start";
            startToolStripMenuItem.Click += startToolStripMenuItem_Click;
            // 
            // pauseToolStripMenuItem
            // 
            pauseToolStripMenuItem.Name = "pauseToolStripMenuItem";
            pauseToolStripMenuItem.Size = new Size(116, 22);
            pauseToolStripMenuItem.Text = "Pause";
            pauseToolStripMenuItem.Click += pauseToolStripMenuItem_Click;
            // 
            // resumeToolStripMenuItem
            // 
            resumeToolStripMenuItem.Name = "resumeToolStripMenuItem";
            resumeToolStripMenuItem.Size = new Size(116, 22);
            resumeToolStripMenuItem.Text = "Resume";
            resumeToolStripMenuItem.Click += resumeToolStripMenuItem_Click;
            // 
            // contextObjectEditor
            // 
            contextObjectEditor.Items.AddRange(new ToolStripItem[] { focusToolStripMenuItem1, destroyToolStripMenuItem1, clearToolStripMenuItem1 });
            contextObjectEditor.Name = "contextObjectEditor";
            contextObjectEditor.Size = new Size(115, 70);
            // 
            // focusToolStripMenuItem1
            // 
            focusToolStripMenuItem1.Name = "focusToolStripMenuItem1";
            focusToolStripMenuItem1.Size = new Size(114, 22);
            focusToolStripMenuItem1.Text = "Focus";
            focusToolStripMenuItem1.Click += focusToolStripMenuItem1_Click;
            // 
            // destroyToolStripMenuItem1
            // 
            destroyToolStripMenuItem1.Name = "destroyToolStripMenuItem1";
            destroyToolStripMenuItem1.Size = new Size(114, 22);
            destroyToolStripMenuItem1.Text = "Destroy";
            destroyToolStripMenuItem1.Click += destroyToolStripMenuItem1_Click;
            // 
            // clearToolStripMenuItem1
            // 
            clearToolStripMenuItem1.Name = "clearToolStripMenuItem1";
            clearToolStripMenuItem1.Size = new Size(114, 22);
            clearToolStripMenuItem1.Text = "Clear";
            clearToolStripMenuItem1.Click += clearToolStripMenuItem1_Click;
            // 
            // ConsoeListBoxContextMenu
            // 
            ConsoeListBoxContextMenu.Items.AddRange(new ToolStripItem[] { clearToolStripMenuItem });
            ConsoeListBoxContextMenu.Name = "ConsoeListBoxContextMenu";
            ConsoeListBoxContextMenu.Size = new Size(102, 26);
            // 
            // clearToolStripMenuItem
            // 
            clearToolStripMenuItem.Name = "clearToolStripMenuItem";
            clearToolStripMenuItem.Size = new Size(101, 22);
            clearToolStripMenuItem.Text = "Clear";
            // 
            // MainForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(562, 727);
            Controls.Add(menuStrip1);
            Controls.Add(tabControl1);
            Name = "MainForm";
            Text = "IDAS-Console";
            TopMost = true;
            ActorListContextMenu.ResumeLayout(false);
            contextMenuConsoleListBox.ResumeLayout(false);
            tabPage2.ResumeLayout(false);
            ObjectEditorContextMenu.ResumeLayout(false);
            flowLayoutPanel1.ResumeLayout(false);
            flowLayoutPanel1.PerformLayout();
            groupBox4.ResumeLayout(false);
            groupBox4.PerformLayout();
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            groupBox3.ResumeLayout(false);
            groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)ModifyableDataGrid).EndInit();
            tabControl1.ResumeLayout(false);
            tabPage1.ResumeLayout(false);
            tabPage1.PerformLayout();
            tabPage3.ResumeLayout(false);
            tabPage3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)WaypointGrid).EndInit();
            PathContextMenu.ResumeLayout(false);
            tabPage4.ResumeLayout(false);
            tabPage4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)TBWindAzimuth).EndInit();
            ((System.ComponentModel.ISupportInitialize)TBWind).EndInit();
            ((System.ComponentModel.ISupportInitialize)TBTimeOfDay).EndInit();
            ((System.ComponentModel.ISupportInitialize)TBFog).EndInit();
            ((System.ComponentModel.ISupportInitialize)TBDust).EndInit();
            ((System.ComponentModel.ISupportInitialize)BThunder).EndInit();
            ((System.ComponentModel.ISupportInitialize)TBSnow).EndInit();
            ((System.ComponentModel.ISupportInitialize)TBRain).EndInit();
            ((System.ComponentModel.ISupportInitialize)TBCloud).EndInit();
            tabPage5.ResumeLayout(false);
            tabPage5.PerformLayout();
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            contextObjectEditor.ResumeLayout(false);
            ConsoeListBoxContextMenu.ResumeLayout(false);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private OpenFileDialog OpenFileDialog;
        private ContextMenuStrip contextMenuStrip1;
        private TextBox CommandTextBox;
        private ListBox listBox1;
        private System.Windows.Forms.Timer SensorTimer;
        private ContextMenuStrip ActorListContextMenu;
        private ToolStripMenuItem toolStripMenuItem3;
        private ToolStripMenuItem loadModelsToolStripMenuItem;
        private TabPage tabPage2;
        private TabControl tabControl1;
        private TabPage tabPage1;
        private ToolStripMenuItem createToolStripMenuItem;
        private FlowLayoutPanel flowLayoutPanel1;
        private TextBox TBName;
        private Button SetActiveButton;
        private CheckBox CBActive;
        private Button SensorEnableCtrlButton;
        private CheckBox CBEnable;
        private Button SetInstanceNo;
        private ComboBox InstanceComboBox;
        private GroupBox groupBox1;
        private Label label3;
        private Label label2;
        private Label label1;
        private TextBox TB_LocationZ;
        private TextBox TB_LocationY;
        private TextBox TB_LocationX;
        private GroupBox groupBox2;
        private Label label4;
        private Label label5;
        private Label label6;
        private TextBox TB_RotationZ;
        private TextBox TB_RotationY;
        private TextBox TB_RotationX;
        private GroupBox groupBox3;
        private Label label7;
        private Label label8;
        private Label label9;
        private TextBox TB_ScaleZ;
        private TextBox TB_ScaleY;
        private TextBox TB_ScaleX;
        private Label label10;
        private GroupBox groupBox4;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem loadWorkspaceToolStripMenuItem;
        private ToolStripMenuItem simulationToolStripMenuItem;
        private ToolStripMenuItem startToolStripMenuItem;
        private ToolStripMenuItem pauseToolStripMenuItem;
        private TreeView ObjectEditor;
        private ContextMenuStrip contextObjectEditor;
        private ToolStripMenuItem focusToolStripMenuItem1;
        private ToolStripMenuItem destroyToolStripMenuItem1;
        private ToolStripMenuItem clearToolStripMenuItem1;
        private ToolStripMenuItem loadActorsToolStripMenuItem;
        private ToolStripMenuItem loadUserActorsToolStripMenuItem;
        private ContextMenuStrip ObjectEditorContextMenu;
        private ToolStripMenuItem toolStripMenuItem2;
        private ToolStripMenuItem viewToolStripMenuItem;
        private ToolStripMenuItem clearAllToolStripMenuItem;
        private DataGridView ModifyableDataGrid;
        private ContextMenuStrip ConsoeListBoxContextMenu;
        private ToolStripMenuItem clearToolStripMenuItem;
        private ContextMenuStrip contextMenuConsoleListBox;
        private ToolStripMenuItem clearCommandsToolStripMenuItem;
        private TabPage tabPage3;
        private DataGridView WaypointGrid;
        private Button button1;
        private TextBox TbTurnRate;
        private Label label14;
        private TextBox TbPathSpeed;
        private Label label13;
        private CheckBox CBIsCurved;
        private CheckBox CBIsLooped;
        private Label label12;
        private TextBox TbPathName;
        private Label label11;
        private TextBox TbPathSegmentCount;
        private Label label15;
        private ContextMenuStrip PathContextMenu;
        private ToolStripMenuItem addToolStripMenuItem;
        private ToolStripMenuItem removeToolStripMenuItem;
        private DataGridViewTextBoxColumn Column2;
        private DataGridViewTextBoxColumn Column3;
        private DataGridViewTextBoxColumn Column4;
        private ToolStripMenuItem blueprintToolStripMenuItem;
        private ToolStripMenuItem attachToPathToolStripMenuItem;
        private ToolStripMenuItem destroyToolStripMenuItem;
        private ToolStripMenuItem resumeToolStripMenuItem;
        private SaveFileDialog FileSaver;
        private ToolStripMenuItem saveWorkspaceToolStripMenuItem;
        private ToolStripMenuItem toolStripMenuItem4;
        private ToolStripMenuItem showChartsToolStripMenuItem;
        private ToolStripMenuItem hideChartsToolStripMenuItem;
        private ToolStripMenuItem detachPathToolStripMenuItem;
        private TabPage tabPage4;
        private TrackBar TBCloud;
        private Label label20;
        private TrackBar TBDust;
        private Label label19;
        private TrackBar BThunder;
        private Label label18;
        private TrackBar TBSnow;
        private Label label17;
        private TrackBar TBRain;
        private Label label16;
        private Label label21;
        private TrackBar TBFog;
        private Label label22;
        private TrackBar TBTimeOfDay;
        private Label LBTimeValue;
        private Label label23;
        private TrackBar TBWind;
        private Label label24;
        private TrackBar TBWindAzimuth;
        private TabPage tabPage5;
        private TextBox TBHMapName;
        private Label label25;
        private TextBox TBHMapMaxLevel;
        private Label label32;
        private TextBox TBHMapMinLevel;
        private Label label31;
        private TextBox TBDMapTexture;
        private Label label27;
        private TextBox TBImaryTexture;
        private Label LBImaryTexture;
        private ComboBox CBCoordinateSystem;
        private TextBox TBCoordZ;
        private TextBox TBCoordY;
        private TextBox TBTopLeftLat;
        private Button BTCreate;
        private TextBox TBTerrainName;
        private Label label26;
        private TextBox TBDMapMaxLevel;
        private Label label34;
        private TextBox TBDMapMinLevel;
        private Label label35;
        private ToolStripMenuItem cameaToolStripMenuItem;
        private ToolStripMenuItem speedToolStripMenuItem;
        private ToolStripMenuItem xToolStripMenuItem;
        private ToolStripMenuItem xToolStripMenuItem1;
        private ToolStripMenuItem xToolStripMenuItem2;
        private ToolStripMenuItem xToolStripMenuItem3;
        private ToolStripMenuItem xToolStripMenuItem4;
        private ToolStripMenuItem xToolStripMenuItem5;
        private ToolStripMenuItem xToolStripMenuItem6;
        private ToolStripMenuItem xToolStripMenuItem7;
        private ToolStripMenuItem xToolStripMenuItem8;
        private TextBox TBTTopLeftH;
        private TextBox TBTTopLeftLon;
        private TextBox TBTTopLeftLat;
        private TextBox TBTBottomRightH;
        private TextBox TBTBottomRightLon;
        private TextBox TBTBottomRightLat;
        private Label label30;
        private Label label33;
        private ToolStripMenuItem viewToolStripMenuItem1;
        private ToolStripMenuItem topToolStripMenuItem1;
        private ToolStripMenuItem leftToolStripMenuItem1;
        private ToolStripMenuItem rightToolStripMenuItem1;
        private ToolStripMenuItem frontToolStripMenuItem;
        private ToolStripMenuItem backToolStripMenuItem;
    }
}