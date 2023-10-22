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
            openFileDialog1 = new OpenFileDialog();
            contextMenuStrip1 = new ContextMenuStrip(components);
            button1 = new Button();
            tabControl1 = new TabControl();
            tabPage2 = new TabPage();
            groupBox3 = new GroupBox();
            label7 = new Label();
            label8 = new Label();
            label9 = new Label();
            TB_ScaleZ = new TextBox();
            TB_ScaleY = new TextBox();
            TB_ScaleX = new TextBox();
            groupBox2 = new GroupBox();
            label4 = new Label();
            label5 = new Label();
            label6 = new Label();
            TB_RotationZ = new TextBox();
            TB_RotationY = new TextBox();
            TB_RotationX = new TextBox();
            groupBox1 = new GroupBox();
            label3 = new Label();
            label2 = new Label();
            label1 = new Label();
            TB_LocationZ = new TextBox();
            TB_LocationY = new TextBox();
            TB_LocationX = new TextBox();
            SetActiveButton = new Button();
            IsActiveCB = new CheckBox();
            SensorEnableCtrlButton = new Button();
            SensorEnabledCB = new CheckBox();
            SetInstanceNo = new Button();
            InstanceComboBox = new ComboBox();
            SensorListBox = new ListBox();
            CommandTextBox = new TextBox();
            listBox1 = new ListBox();
            SensorTimer = new System.Windows.Forms.Timer(components);
            tabControl1.SuspendLayout();
            tabPage2.SuspendLayout();
            groupBox3.SuspendLayout();
            groupBox2.SuspendLayout();
            groupBox1.SuspendLayout();
            SuspendLayout();
            // 
            // openFileDialog1
            // 
            openFileDialog1.FileName = "openFileDialog1";
            openFileDialog1.FileOk += openFileDialog1_FileOk;
            // 
            // contextMenuStrip1
            // 
            contextMenuStrip1.Name = "contextMenuStrip1";
            contextMenuStrip1.Size = new Size(61, 4);
            // 
            // button1
            // 
            button1.Location = new Point(664, 12);
            button1.Name = "button1";
            button1.Size = new Size(75, 23);
            button1.TabIndex = 3;
            button1.Text = "Run";
            button1.UseVisualStyleBackColor = true;
            // 
            // tabControl1
            // 
            tabControl1.Controls.Add(tabPage2);
            tabControl1.Location = new Point(542, 0);
            tabControl1.Name = "tabControl1";
            tabControl1.SelectedIndex = 0;
            tabControl1.Size = new Size(522, 575);
            tabControl1.TabIndex = 4;
            // 
            // tabPage2
            // 
            tabPage2.Controls.Add(groupBox3);
            tabPage2.Controls.Add(groupBox2);
            tabPage2.Controls.Add(groupBox1);
            tabPage2.Controls.Add(SetActiveButton);
            tabPage2.Controls.Add(IsActiveCB);
            tabPage2.Controls.Add(SensorEnableCtrlButton);
            tabPage2.Controls.Add(SensorEnabledCB);
            tabPage2.Controls.Add(SetInstanceNo);
            tabPage2.Controls.Add(InstanceComboBox);
            tabPage2.Controls.Add(SensorListBox);
            tabPage2.Location = new Point(4, 24);
            tabPage2.Name = "tabPage2";
            tabPage2.Padding = new Padding(3);
            tabPage2.Size = new Size(514, 547);
            tabPage2.TabIndex = 1;
            tabPage2.Text = "Sensors";
            tabPage2.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            groupBox3.Controls.Add(label7);
            groupBox3.Controls.Add(label8);
            groupBox3.Controls.Add(label9);
            groupBox3.Controls.Add(TB_ScaleZ);
            groupBox3.Controls.Add(TB_ScaleY);
            groupBox3.Controls.Add(TB_ScaleX);
            groupBox3.Location = new Point(379, 423);
            groupBox3.Name = "groupBox3";
            groupBox3.Size = new Size(130, 111);
            groupBox3.TabIndex = 14;
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
            TB_ScaleZ.KeyDown += TB_ScaleZ_KeyDown;
            // 
            // TB_ScaleY
            // 
            TB_ScaleY.Location = new Point(24, 51);
            TB_ScaleY.Name = "TB_ScaleY";
            TB_ScaleY.Size = new Size(100, 23);
            TB_ScaleY.TabIndex = 8;
            TB_ScaleY.KeyDown += TB_ScaleY_KeyDown;
            // 
            // TB_ScaleX
            // 
            TB_ScaleX.Location = new Point(24, 23);
            TB_ScaleX.Name = "TB_ScaleX";
            TB_ScaleX.Size = new Size(100, 23);
            TB_ScaleX.TabIndex = 7;
            TB_ScaleX.KeyDown += TB_ScaleX_KeyDown;
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(label4);
            groupBox2.Controls.Add(label5);
            groupBox2.Controls.Add(label6);
            groupBox2.Controls.Add(TB_RotationZ);
            groupBox2.Controls.Add(TB_RotationY);
            groupBox2.Controls.Add(TB_RotationX);
            groupBox2.Location = new Point(378, 306);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(130, 111);
            groupBox2.TabIndex = 13;
            groupBox2.TabStop = false;
            groupBox2.Text = "Rotation";
            groupBox2.Enter += groupBox2_Enter;
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
            TB_RotationZ.KeyDown += TB_RotationZ_KeyDown;
            // 
            // TB_RotationY
            // 
            TB_RotationY.Location = new Point(24, 51);
            TB_RotationY.Name = "TB_RotationY";
            TB_RotationY.Size = new Size(100, 23);
            TB_RotationY.TabIndex = 8;
            TB_RotationY.KeyDown += TB_RotationY_KeyDown;
            // 
            // TB_RotationX
            // 
            TB_RotationX.Location = new Point(24, 23);
            TB_RotationX.Name = "TB_RotationX";
            TB_RotationX.Size = new Size(100, 23);
            TB_RotationX.TabIndex = 7;
            TB_RotationX.KeyDown += TB_RotationX_KeyDown;
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(label3);
            groupBox1.Controls.Add(label2);
            groupBox1.Controls.Add(label1);
            groupBox1.Controls.Add(TB_LocationZ);
            groupBox1.Controls.Add(TB_LocationY);
            groupBox1.Controls.Add(TB_LocationX);
            groupBox1.Location = new Point(375, 189);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(130, 111);
            groupBox1.TabIndex = 8;
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
            label2.Click += label2_Click;
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
            TB_LocationZ.TextChanged += TB_LocationZ_TextChanged;
            TB_LocationZ.KeyDown += TB_LocationZ_KeyDown;
            // 
            // TB_LocationY
            // 
            TB_LocationY.Location = new Point(24, 51);
            TB_LocationY.Name = "TB_LocationY";
            TB_LocationY.Size = new Size(100, 23);
            TB_LocationY.TabIndex = 8;
            TB_LocationY.KeyDown += TB_LocationY_KeyDown;
            // 
            // TB_LocationX
            // 
            TB_LocationX.Location = new Point(24, 23);
            TB_LocationX.Name = "TB_LocationX";
            TB_LocationX.Size = new Size(100, 23);
            TB_LocationX.TabIndex = 7;
            TB_LocationX.TextChanged += TB_LocationX_TextChanged;
            TB_LocationX.KeyDown += TB_LocationX_KeyDown;
            TB_LocationX.Leave += TB_LocationX_Leave;
            TB_LocationX.MouseLeave += TB_LocationX_MouseLeave;
            // 
            // SetActiveButton
            // 
            SetActiveButton.Location = new Point(375, 160);
            SetActiveButton.Name = "SetActiveButton";
            SetActiveButton.Size = new Size(130, 23);
            SetActiveButton.TabIndex = 6;
            SetActiveButton.Text = "Set Active";
            SetActiveButton.UseVisualStyleBackColor = true;
            SetActiveButton.Click += SetActiveButton_Click;
            // 
            // IsActiveCB
            // 
            IsActiveCB.AutoSize = true;
            IsActiveCB.Location = new Point(375, 135);
            IsActiveCB.Name = "IsActiveCB";
            IsActiveCB.Size = new Size(66, 19);
            IsActiveCB.TabIndex = 5;
            IsActiveCB.Text = "Enable?";
            IsActiveCB.UseVisualStyleBackColor = true;
            // 
            // SensorEnableCtrlButton
            // 
            SensorEnableCtrlButton.Location = new Point(375, 96);
            SensorEnableCtrlButton.Name = "SensorEnableCtrlButton";
            SensorEnableCtrlButton.Size = new Size(130, 23);
            SensorEnableCtrlButton.TabIndex = 4;
            SensorEnableCtrlButton.Text = "SetEnable";
            SensorEnableCtrlButton.UseVisualStyleBackColor = true;
            SensorEnableCtrlButton.Click += SensorEnableCtrlButton_Click;
            // 
            // SensorEnabledCB
            // 
            SensorEnabledCB.AutoSize = true;
            SensorEnabledCB.Location = new Point(375, 71);
            SensorEnabledCB.Name = "SensorEnabledCB";
            SensorEnabledCB.Size = new Size(66, 19);
            SensorEnabledCB.TabIndex = 3;
            SensorEnabledCB.Text = "Enable?";
            SensorEnabledCB.UseVisualStyleBackColor = true;
            // 
            // SetInstanceNo
            // 
            SetInstanceNo.Location = new Point(375, 40);
            SetInstanceNo.Name = "SetInstanceNo";
            SetInstanceNo.Size = new Size(130, 23);
            SetInstanceNo.TabIndex = 2;
            SetInstanceNo.Text = "Set Instance";
            SetInstanceNo.UseVisualStyleBackColor = true;
            SetInstanceNo.Click += SetInstanceNo_Click;
            // 
            // InstanceComboBox
            // 
            InstanceComboBox.FormattingEnabled = true;
            InstanceComboBox.Items.AddRange(new object[] { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15" });
            InstanceComboBox.Location = new Point(375, 11);
            InstanceComboBox.Name = "InstanceComboBox";
            InstanceComboBox.Size = new Size(130, 23);
            InstanceComboBox.TabIndex = 1;
            // 
            // SensorListBox
            // 
            SensorListBox.Font = new Font("Segoe UI", 8F, FontStyle.Regular, GraphicsUnit.Point);
            SensorListBox.FormattingEnabled = true;
            SensorListBox.Location = new Point(13, 11);
            SensorListBox.Name = "SensorListBox";
            SensorListBox.Size = new Size(356, 511);
            SensorListBox.TabIndex = 0;
            SensorListBox.MouseDoubleClick += SensorListBox_MouseDoubleClick;
            // 
            // CommandTextBox
            // 
            CommandTextBox.Location = new Point(10, 548);
            CommandTextBox.Name = "CommandTextBox";
            CommandTextBox.Size = new Size(526, 23);
            CommandTextBox.TabIndex = 4;
            CommandTextBox.KeyDown += CommandTextBox_KeyDown;
            // 
            // listBox1
            // 
            listBox1.FormattingEnabled = true;
            listBox1.HorizontalScrollbar = true;
            listBox1.ItemHeight = 15;
            listBox1.Location = new Point(10, 15);
            listBox1.Name = "listBox1";
            listBox1.Size = new Size(526, 514);
            listBox1.TabIndex = 3;
            listBox1.Click += listBox1_Click;
            listBox1.MouseClick += listBox1_MouseClick_1;
            listBox1.MouseDoubleClick += listBox1_MouseDoubleClick_1;
            // 
            // SensorTimer
            // 
            SensorTimer.Enabled = true;
            SensorTimer.Interval = 1000;
            SensorTimer.Tick += timer1_Tick_1;
            // 
            // MainForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1088, 587);
            Controls.Add(listBox1);
            Controls.Add(CommandTextBox);
            Controls.Add(tabControl1);
            Controls.Add(button1);
            Name = "MainForm";
            Text = "Sim Console";
            tabControl1.ResumeLayout(false);
            tabPage2.ResumeLayout(false);
            tabPage2.PerformLayout();
            groupBox3.ResumeLayout(false);
            groupBox3.PerformLayout();
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private OpenFileDialog openFileDialog1;
        private ContextMenuStrip contextMenuStrip1;
        private Button button1;
        private TabControl tabControl1;
        private TabPage tabPage2;
        private TextBox CommandTextBox;
        private ListBox listBox1;
        private Button SetInstanceNo;
        private ComboBox InstanceComboBox;
        private ListBox SensorListBox;
        private Button SensorEnableCtrlButton;
        private CheckBox SensorEnabledCB;
        private Button SetActiveButton;
        private CheckBox IsActiveCB;
        private GroupBox groupBox1;
        private Label label2;
        private Label label1;
        private TextBox TB_LocationZ;
        private TextBox TB_LocationY;
        private TextBox TB_LocationX;
        private Label label3;
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
        private System.Windows.Forms.Timer SensorTimer;
    }
}