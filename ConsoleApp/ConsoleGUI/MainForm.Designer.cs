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
            OpenFileDialog = new OpenFileDialog();
            contextMenuStrip1 = new ContextMenuStrip(components);
            ActorListContextMenu = new ContextMenuStrip(components);
            createToolStripMenuItem = new ToolStripMenuItem();
            CommandTextBox = new TextBox();
            listBox1 = new ListBox();
            SensorTimer = new System.Windows.Forms.Timer(components);
            tabPage2 = new TabPage();
            ObjectEditor = new TreeView();
            ObjectEditorContextMenu = new ContextMenuStrip(components);
            createToolStripMenuItem1 = new ToolStripMenuItem();
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
            ActorGrid = new DataGridView();
            ClType = new DataGridViewTextBoxColumn();
            ClName = new DataGridViewTextBoxColumn();
            tabControl1 = new TabControl();
            tabPage1 = new TabPage();
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
            contextObjectEditor = new ContextMenuStrip(components);
            focusToolStripMenuItem1 = new ToolStripMenuItem();
            destroyToolStripMenuItem1 = new ToolStripMenuItem();
            clearToolStripMenuItem1 = new ToolStripMenuItem();
            ActorListContextMenu.SuspendLayout();
            tabPage2.SuspendLayout();
            ObjectEditorContextMenu.SuspendLayout();
            flowLayoutPanel1.SuspendLayout();
            groupBox4.SuspendLayout();
            groupBox1.SuspendLayout();
            groupBox2.SuspendLayout();
            groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)ActorGrid).BeginInit();
            tabControl1.SuspendLayout();
            tabPage1.SuspendLayout();
            menuStrip1.SuspendLayout();
            contextObjectEditor.SuspendLayout();
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
            tabPage2.Controls.Add(ActorGrid);
            tabPage2.Location = new Point(4, 24);
            tabPage2.Name = "tabPage2";
            tabPage2.Padding = new Padding(3);
            tabPage2.Size = new Size(553, 547);
            tabPage2.TabIndex = 1;
            tabPage2.Text = "Actors";
            tabPage2.UseVisualStyleBackColor = true;
            // 
            // ObjectEditor
            // 
            ObjectEditor.ContextMenuStrip = ObjectEditorContextMenu;
            ObjectEditor.Location = new Point(13, 147);
            ObjectEditor.Name = "ObjectEditor";
            treeNode1.Name = "World";
            treeNode1.Text = "World";
            ObjectEditor.Nodes.AddRange(new TreeNode[] { treeNode1 });
            ObjectEditor.Size = new Size(356, 370);
            ObjectEditor.TabIndex = 18;
            ObjectEditor.NodeMouseClick += ObjectEditor_NodeMouseClick;
            ObjectEditor.MouseClick += ObjectEditor_MouseClick;
            // 
            // ObjectEditorContextMenu
            // 
            ObjectEditorContextMenu.Items.AddRange(new ToolStripItem[] { createToolStripMenuItem1 });
            ObjectEditorContextMenu.Name = "contextMenuStrip2";
            ObjectEditorContextMenu.Size = new Size(109, 26);
            // 
            // createToolStripMenuItem1
            // 
            createToolStripMenuItem1.Name = "createToolStripMenuItem1";
            createToolStripMenuItem1.Size = new Size(108, 22);
            createToolStripMenuItem1.Text = "Create";
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
            flowLayoutPanel1.Location = new Point(386, 11);
            flowLayoutPanel1.Name = "flowLayoutPanel1";
            flowLayoutPanel1.Size = new Size(160, 531);
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
            groupBox4.Size = new Size(130, 149);
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
            groupBox1.Size = new Size(130, 111);
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
            TB_LocationZ.Size = new Size(100, 23);
            TB_LocationZ.TabIndex = 9;
            TB_LocationZ.Text = "0";
            TB_LocationZ.KeyDown += TB_LocationZ_KeyDown;
            // 
            // TB_LocationY
            // 
            TB_LocationY.Location = new Point(24, 51);
            TB_LocationY.Name = "TB_LocationY";
            TB_LocationY.Size = new Size(100, 23);
            TB_LocationY.TabIndex = 8;
            TB_LocationY.Text = "0";
            TB_LocationY.KeyDown += TB_LocationY_KeyDown;
            // 
            // TB_LocationX
            // 
            TB_LocationX.Location = new Point(24, 23);
            TB_LocationX.Name = "TB_LocationX";
            TB_LocationX.Size = new Size(100, 23);
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
            groupBox2.Size = new Size(130, 111);
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
            TB_RotationZ.Size = new Size(100, 23);
            TB_RotationZ.TabIndex = 9;
            TB_RotationZ.Text = "0";
            TB_RotationZ.KeyDown += TB_RotationZ_KeyDown;
            // 
            // TB_RotationY
            // 
            TB_RotationY.Location = new Point(24, 51);
            TB_RotationY.Name = "TB_RotationY";
            TB_RotationY.Size = new Size(100, 23);
            TB_RotationY.TabIndex = 8;
            TB_RotationY.Text = "0";
            TB_RotationY.KeyDown += TB_RotationY_KeyDown;
            // 
            // TB_RotationX
            // 
            TB_RotationX.Location = new Point(24, 23);
            TB_RotationX.Name = "TB_RotationX";
            TB_RotationX.Size = new Size(100, 23);
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
            groupBox3.Size = new Size(130, 111);
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
            TB_ScaleZ.Size = new Size(100, 23);
            TB_ScaleZ.TabIndex = 9;
            TB_ScaleZ.Text = "1";
            TB_ScaleZ.KeyDown += TB_ScaleZ_KeyDown;
            // 
            // TB_ScaleY
            // 
            TB_ScaleY.Location = new Point(24, 51);
            TB_ScaleY.Name = "TB_ScaleY";
            TB_ScaleY.Size = new Size(100, 23);
            TB_ScaleY.TabIndex = 8;
            TB_ScaleY.Text = "1";
            TB_ScaleY.KeyDown += TB_ScaleY_KeyDown;
            // 
            // TB_ScaleX
            // 
            TB_ScaleX.Location = new Point(24, 23);
            TB_ScaleX.Name = "TB_ScaleX";
            TB_ScaleX.Size = new Size(100, 23);
            TB_ScaleX.TabIndex = 7;
            TB_ScaleX.Text = "1";
            TB_ScaleX.KeyDown += TB_ScaleX_KeyDown;
            // 
            // ActorGrid
            // 
            ActorGrid.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            ActorGrid.Columns.AddRange(new DataGridViewColumn[] { ClType, ClName });
            ActorGrid.ContextMenuStrip = ActorListContextMenu;
            ActorGrid.Location = new Point(13, 11);
            ActorGrid.MultiSelect = false;
            ActorGrid.Name = "ActorGrid";
            ActorGrid.ReadOnly = true;
            ActorGrid.RowHeadersWidth = 50;
            ActorGrid.RowTemplate.Height = 25;
            ActorGrid.RowTemplate.ReadOnly = true;
            ActorGrid.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            ActorGrid.Size = new Size(356, 130);
            ActorGrid.TabIndex = 15;
            ActorGrid.MouseClick += ActorGrid_MouseClick;
            ActorGrid.MouseDoubleClick += ActorGrid_MouseDoubleClick;
            // 
            // ClType
            // 
            ClType.HeaderText = "Type";
            ClType.Name = "ClType";
            ClType.ReadOnly = true;
            ClType.Width = 50;
            // 
            // ClName
            // 
            ClName.HeaderText = "Name";
            ClName.Name = "ClName";
            ClName.ReadOnly = true;
            ClName.Width = 400;
            // 
            // tabControl1
            // 
            tabControl1.Controls.Add(tabPage2);
            tabControl1.Controls.Add(tabPage1);
            tabControl1.Location = new Point(5, 27);
            tabControl1.Name = "tabControl1";
            tabControl1.SelectedIndex = 0;
            tabControl1.Size = new Size(561, 575);
            tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            tabPage1.Controls.Add(listBox1);
            tabPage1.Controls.Add(CommandTextBox);
            tabPage1.Location = new Point(4, 24);
            tabPage1.Name = "tabPage1";
            tabPage1.Padding = new Padding(3);
            tabPage1.Size = new Size(553, 547);
            tabPage1.TabIndex = 2;
            tabPage1.Text = "Console";
            tabPage1.UseVisualStyleBackColor = true;
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, viewToolStripMenuItem, simulationToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(566, 24);
            menuStrip1.TabIndex = 5;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { loadWorkspaceToolStripMenuItem, loadBlueprintsToolStripMenuItem, loadUserActorsToolStripMenuItem, toolStripMenuItem2, loadActorsToolStripMenuItem });
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
            // loadBlueprintsToolStripMenuItem
            // 
            loadBlueprintsToolStripMenuItem.Name = "loadBlueprintsToolStripMenuItem";
            loadBlueprintsToolStripMenuItem.Size = new Size(163, 22);
            loadBlueprintsToolStripMenuItem.Text = "Load Blueprints";
            loadBlueprintsToolStripMenuItem.Click += loadBlueprintsToolStripMenuItem_Click;
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
            simulationToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { startToolStripMenuItem, pauseToolStripMenuItem });
            simulationToolStripMenuItem.Name = "simulationToolStripMenuItem";
            simulationToolStripMenuItem.Size = new Size(76, 20);
            simulationToolStripMenuItem.Text = "Simulation";
            // 
            // startToolStripMenuItem
            // 
            startToolStripMenuItem.Name = "startToolStripMenuItem";
            startToolStripMenuItem.Size = new Size(105, 22);
            startToolStripMenuItem.Text = "Start";
            startToolStripMenuItem.Click += startToolStripMenuItem_Click;
            // 
            // pauseToolStripMenuItem
            // 
            pauseToolStripMenuItem.Name = "pauseToolStripMenuItem";
            pauseToolStripMenuItem.Size = new Size(105, 22);
            pauseToolStripMenuItem.Text = "Pause";
            pauseToolStripMenuItem.Click += pauseToolStripMenuItem_Click;
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
            // MainForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(566, 591);
            Controls.Add(menuStrip1);
            Controls.Add(tabControl1);
            Name = "MainForm";
            Text = "Sim Console";
            TopMost = true;
            ActorListContextMenu.ResumeLayout(false);
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
            ((System.ComponentModel.ISupportInitialize)ActorGrid).EndInit();
            tabControl1.ResumeLayout(false);
            tabPage1.ResumeLayout(false);
            tabPage1.PerformLayout();
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            contextObjectEditor.ResumeLayout(false);
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
        private DataGridView ActorGrid;
        private DataGridViewTextBoxColumn ClType;
        private DataGridViewTextBoxColumn ClName;
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
        private ToolStripMenuItem createToolStripMenuItem1;
        private ToolStripMenuItem loadBlueprintsToolStripMenuItem;
        private ToolStripMenuItem toolStripMenuItem2;
        private ToolStripMenuItem viewToolStripMenuItem;
        private ToolStripMenuItem clearAllToolStripMenuItem;
    }
}