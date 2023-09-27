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
            SensorEnableCtrlButton = new Button();
            SensorEnabledCB = new CheckBox();
            SetInstanceNo = new Button();
            InstanceComboBox = new ComboBox();
            SensorListBox = new ListBox();
            CommandTextBox = new TextBox();
            listBox1 = new ListBox();
            SetActiveButton = new Button();
            IsActiveCB = new CheckBox();
            tabControl1.SuspendLayout();
            tabPage2.SuspendLayout();
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
            // SensorEnableCtrlButton
            // 
            SensorEnableCtrlButton.Location = new Point(375, 110);
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
            SensorEnabledCB.Location = new Point(375, 85);
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
            // SetActiveButton
            // 
            SetActiveButton.Location = new Point(375, 174);
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
            IsActiveCB.Location = new Point(375, 149);
            IsActiveCB.Name = "IsActiveCB";
            IsActiveCB.Size = new Size(66, 19);
            IsActiveCB.TabIndex = 5;
            IsActiveCB.Text = "Enable?";
            IsActiveCB.UseVisualStyleBackColor = true;
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
    }
}