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
            TreeNode treeNode2 = new TreeNode("World");
            DataGridViewCellStyle dataGridViewCellStyle4 = new DataGridViewCellStyle();
            DataGridViewCellStyle dataGridViewCellStyle5 = new DataGridViewCellStyle();
            DataGridViewCellStyle dataGridViewCellStyle6 = new DataGridViewCellStyle();
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
            menuStrip1 = new MenuStrip();
            fileToolStripMenuItem = new ToolStripMenuItem();
            loadWorkspaceToolStripMenuItem = new ToolStripMenuItem();
            loadBlueprintsToolStripMenuItem = new ToolStripMenuItem();
            loadUserActorsToolStripMenuItem = new ToolStripMenuItem();
            toolStripMenuItem2 = new ToolStripMenuItem();
            loadActorsToolStripMenuItem = new ToolStripMenuItem();
            viewToolStripMenuItem = new ToolStripMenuItem();
            clearAllToolStripMenuItem = new ToolStripMenuItem();
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
            saveWorkspaceToolStripMenuItem = new ToolStripMenuItem();
            toolStripMenuItem4 = new ToolStripMenuItem();
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
            tabPage2.Size = new Size(502, 650);
            tabPage2.TabIndex = 1;
            tabPage2.Text = "Actors";
            tabPage2.UseVisualStyleBackColor = true;
            // 
            // ObjectEditor
            // 
            ObjectEditor.ContextMenuStrip = ObjectEditorContextMenu;
            ObjectEditor.Location = new Point(13, 11);
            ObjectEditor.Name = "ObjectEditor";
            treeNode2.Name = "World";
            treeNode2.Text = "World";
            ObjectEditor.Nodes.AddRange(new TreeNode[] { treeNode2 });
            ObjectEditor.Size = new Size(246, 633);
            ObjectEditor.TabIndex = 18;
            ObjectEditor.NodeMouseClick += ObjectEditor_NodeMouseClick;
            ObjectEditor.NodeMouseDoubleClick += ObjectEditor_NodeMouseDoubleClick;
            ObjectEditor.MouseClick += ObjectEditor_MouseClick;
            // 
            // ObjectEditorContextMenu
            // 
            ObjectEditorContextMenu.Items.AddRange(new ToolStripItem[] { blueprintToolStripMenuItem, attachToPathToolStripMenuItem, destroyToolStripMenuItem });
            ObjectEditorContextMenu.Name = "contextMenuStrip2";
            ObjectEditorContextMenu.Size = new Size(137, 70);
            ObjectEditorContextMenu.Opening += ObjectEditorContextMenu_Opening;
            // 
            // blueprintToolStripMenuItem
            // 
            blueprintToolStripMenuItem.Name = "blueprintToolStripMenuItem";
            blueprintToolStripMenuItem.Size = new Size(136, 22);
            blueprintToolStripMenuItem.Text = "Blueprints";
            // 
            // attachToPathToolStripMenuItem
            // 
            attachToPathToolStripMenuItem.Name = "attachToPathToolStripMenuItem";
            attachToPathToolStripMenuItem.Size = new Size(136, 22);
            attachToPathToolStripMenuItem.Text = "Attach Path";
            // 
            // destroyToolStripMenuItem
            // 
            destroyToolStripMenuItem.Name = "destroyToolStripMenuItem";
            destroyToolStripMenuItem.Size = new Size(136, 22);
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
            flowLayoutPanel1.Size = new Size(229, 633);
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
            groupBox4.Size = new Size(202, 149);
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
            groupBox1.Size = new Size(202, 111);
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
            TB_LocationZ.Size = new Size(172, 23);
            TB_LocationZ.TabIndex = 9;
            TB_LocationZ.Text = "0";
            TB_LocationZ.KeyDown += TB_LocationZ_KeyDown;
            // 
            // TB_LocationY
            // 
            TB_LocationY.Location = new Point(24, 51);
            TB_LocationY.Name = "TB_LocationY";
            TB_LocationY.Size = new Size(172, 23);
            TB_LocationY.TabIndex = 8;
            TB_LocationY.Text = "0";
            TB_LocationY.KeyDown += TB_LocationY_KeyDown;
            // 
            // TB_LocationX
            // 
            TB_LocationX.Location = new Point(24, 23);
            TB_LocationX.Name = "TB_LocationX";
            TB_LocationX.Size = new Size(172, 23);
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
            groupBox2.Size = new Size(202, 111);
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
            TB_RotationZ.Size = new Size(172, 23);
            TB_RotationZ.TabIndex = 9;
            TB_RotationZ.Text = "0";
            TB_RotationZ.KeyDown += TB_RotationZ_KeyDown;
            // 
            // TB_RotationY
            // 
            TB_RotationY.Location = new Point(24, 51);
            TB_RotationY.Name = "TB_RotationY";
            TB_RotationY.Size = new Size(172, 23);
            TB_RotationY.TabIndex = 8;
            TB_RotationY.Text = "0";
            TB_RotationY.KeyDown += TB_RotationY_KeyDown;
            // 
            // TB_RotationX
            // 
            TB_RotationX.Location = new Point(24, 23);
            TB_RotationX.Name = "TB_RotationX";
            TB_RotationX.Size = new Size(172, 23);
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
            groupBox3.Size = new Size(202, 107);
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
            TB_ScaleZ.Size = new Size(172, 23);
            TB_ScaleZ.TabIndex = 9;
            TB_ScaleZ.Text = "1";
            TB_ScaleZ.KeyDown += TB_ScaleZ_KeyDown;
            // 
            // TB_ScaleY
            // 
            TB_ScaleY.Location = new Point(24, 51);
            TB_ScaleY.Name = "TB_ScaleY";
            TB_ScaleY.Size = new Size(172, 23);
            TB_ScaleY.TabIndex = 8;
            TB_ScaleY.Text = "1";
            TB_ScaleY.KeyDown += TB_ScaleY_KeyDown;
            // 
            // TB_ScaleX
            // 
            TB_ScaleX.Location = new Point(24, 23);
            TB_ScaleX.Name = "TB_ScaleX";
            TB_ScaleX.Size = new Size(172, 23);
            TB_ScaleX.TabIndex = 7;
            TB_ScaleX.Text = "1";
            TB_ScaleX.KeyDown += TB_ScaleX_KeyDown;
            // 
            // ModifyableDataGrid
            // 
            dataGridViewCellStyle4.Alignment = DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle4.BackColor = SystemColors.Control;
            dataGridViewCellStyle4.Font = new Font("Segoe UI", 1F, FontStyle.Regular, GraphicsUnit.Point);
            dataGridViewCellStyle4.ForeColor = SystemColors.WindowText;
            dataGridViewCellStyle4.SelectionBackColor = SystemColors.Highlight;
            dataGridViewCellStyle4.SelectionForeColor = SystemColors.HighlightText;
            dataGridViewCellStyle4.WrapMode = DataGridViewTriState.True;
            ModifyableDataGrid.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle4;
            ModifyableDataGrid.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle5.Alignment = DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle5.BackColor = SystemColors.Window;
            dataGridViewCellStyle5.Font = new Font("Segoe UI", 8F, FontStyle.Regular, GraphicsUnit.Point);
            dataGridViewCellStyle5.ForeColor = SystemColors.ControlText;
            dataGridViewCellStyle5.SelectionBackColor = SystemColors.Highlight;
            dataGridViewCellStyle5.SelectionForeColor = SystemColors.HighlightText;
            dataGridViewCellStyle5.WrapMode = DataGridViewTriState.False;
            ModifyableDataGrid.DefaultCellStyle = dataGridViewCellStyle5;
            ModifyableDataGrid.Location = new Point(3, 534);
            ModifyableDataGrid.Name = "ModifyableDataGrid";
            dataGridViewCellStyle6.Alignment = DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle6.BackColor = SystemColors.Control;
            dataGridViewCellStyle6.Font = new Font("Segoe UI", 6F, FontStyle.Regular, GraphicsUnit.Point);
            dataGridViewCellStyle6.ForeColor = SystemColors.WindowText;
            dataGridViewCellStyle6.SelectionBackColor = SystemColors.Highlight;
            dataGridViewCellStyle6.SelectionForeColor = SystemColors.HighlightText;
            dataGridViewCellStyle6.WrapMode = DataGridViewTriState.True;
            ModifyableDataGrid.RowHeadersDefaultCellStyle = dataGridViewCellStyle6;
            ModifyableDataGrid.RowHeadersVisible = false;
            ModifyableDataGrid.RowTemplate.Height = 25;
            ModifyableDataGrid.Size = new Size(202, 405);
            ModifyableDataGrid.TabIndex = 34;
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
            tabControl1.Location = new Point(6, 27);
            tabControl1.Name = "tabControl1";
            tabControl1.SelectedIndex = 0;
            tabControl1.Size = new Size(510, 678);
            tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            tabPage1.Controls.Add(listBox1);
            tabPage1.Controls.Add(CommandTextBox);
            tabPage1.Location = new Point(4, 24);
            tabPage1.Name = "tabPage1";
            tabPage1.Padding = new Padding(3);
            tabPage1.Size = new Size(502, 650);
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
            tabPage3.Size = new Size(502, 650);
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
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, viewToolStripMenuItem, simulationToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(516, 24);
            menuStrip1.TabIndex = 5;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { loadWorkspaceToolStripMenuItem, saveWorkspaceToolStripMenuItem, toolStripMenuItem4, loadBlueprintsToolStripMenuItem, loadUserActorsToolStripMenuItem, toolStripMenuItem2, loadActorsToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new Size(37, 20);
            fileToolStripMenuItem.Text = "File";
            // 
            // loadWorkspaceToolStripMenuItem
            // 
            loadWorkspaceToolStripMenuItem.Name = "loadWorkspaceToolStripMenuItem";
            loadWorkspaceToolStripMenuItem.Size = new Size(180, 22);
            loadWorkspaceToolStripMenuItem.Text = "Load Workspace";
            loadWorkspaceToolStripMenuItem.Click += loadWorkspaceToolStripMenuItem_Click;
            // 
            // loadBlueprintsToolStripMenuItem
            // 
            loadBlueprintsToolStripMenuItem.Name = "loadBlueprintsToolStripMenuItem";
            loadBlueprintsToolStripMenuItem.Size = new Size(180, 22);
            loadBlueprintsToolStripMenuItem.Text = "Load Blueprints";
            loadBlueprintsToolStripMenuItem.Click += loadBlueprintsToolStripMenuItem_Click;
            // 
            // loadUserActorsToolStripMenuItem
            // 
            loadUserActorsToolStripMenuItem.Name = "loadUserActorsToolStripMenuItem";
            loadUserActorsToolStripMenuItem.Size = new Size(180, 22);
            loadUserActorsToolStripMenuItem.Text = "Load User Actors";
            loadUserActorsToolStripMenuItem.Click += loadUserActorsToolStripMenuItem_Click;
            // 
            // toolStripMenuItem2
            // 
            toolStripMenuItem2.Name = "toolStripMenuItem2";
            toolStripMenuItem2.Size = new Size(180, 22);
            toolStripMenuItem2.Text = "  ";
            // 
            // loadActorsToolStripMenuItem
            // 
            loadActorsToolStripMenuItem.Name = "loadActorsToolStripMenuItem";
            loadActorsToolStripMenuItem.Size = new Size(180, 22);
            loadActorsToolStripMenuItem.Text = "Load All Actors";
            loadActorsToolStripMenuItem.Click += loadActorsToolStripMenuItem_Click;
            // 
            // viewToolStripMenuItem
            // 
            viewToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { clearAllToolStripMenuItem });
            viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            viewToolStripMenuItem.Size = new Size(44, 20);
            viewToolStripMenuItem.Text = "View";
            // 
            // clearAllToolStripMenuItem
            // 
            clearAllToolStripMenuItem.Name = "clearAllToolStripMenuItem";
            clearAllToolStripMenuItem.Size = new Size(118, 22);
            clearAllToolStripMenuItem.Text = "Clear All";
            clearAllToolStripMenuItem.Click += clearAllToolStripMenuItem_Click;
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
            // saveWorkspaceToolStripMenuItem
            // 
            saveWorkspaceToolStripMenuItem.Name = "saveWorkspaceToolStripMenuItem";
            saveWorkspaceToolStripMenuItem.Size = new Size(180, 22);
            saveWorkspaceToolStripMenuItem.Text = "Save Workspace";
            saveWorkspaceToolStripMenuItem.Click += saveWorkspaceToolStripMenuItem_Click;
            // 
            // toolStripMenuItem4
            // 
            toolStripMenuItem4.Name = "toolStripMenuItem4";
            toolStripMenuItem4.Size = new Size(180, 22);
            toolStripMenuItem4.Text = "  ";
            // 
            // MainForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(516, 727);
            Controls.Add(menuStrip1);
            Controls.Add(tabControl1);
            Name = "MainForm";
            Text = "Sim Console";
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
        private ToolStripMenuItem loadBlueprintsToolStripMenuItem;
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
    }
}