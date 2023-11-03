namespace ConsoleGUI
{
    partial class CInputDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            btnOK = new Button();
            btnCancel = new Button();
            txtInput = new TextBox();
            lblPrompt = new Label();
            SuspendLayout();
            // 
            // btnOK
            // 
            btnOK.Location = new Point(137, 57);
            btnOK.Name = "btnOK";
            btnOK.Size = new Size(75, 23);
            btnOK.TabIndex = 0;
            btnOK.Text = "OK";
            btnOK.UseVisualStyleBackColor = true;
            btnOK.Click += btnOK_Click;
            // 
            // btnCancel
            // 
            btnCancel.Location = new Point(218, 57);
            btnCancel.Name = "btnCancel";
            btnCancel.Size = new Size(75, 23);
            btnCancel.TabIndex = 1;
            btnCancel.Text = "CANCEL";
            btnCancel.UseVisualStyleBackColor = true;
            // 
            // txtInput
            // 
            txtInput.Location = new Point(8, 27);
            txtInput.Name = "txtInput";
            txtInput.Size = new Size(284, 23);
            txtInput.TabIndex = 2;
            txtInput.KeyDown += txtInput_KeyDown;
            // 
            // lblPrompt
            // 
            lblPrompt.AutoSize = true;
            lblPrompt.Location = new Point(8, 9);
            lblPrompt.Name = "lblPrompt";
            lblPrompt.Size = new Size(0, 15);
            lblPrompt.TabIndex = 3;
            lblPrompt.Click += lblPrompt_Click;
            // 
            // CInputDialog
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(299, 85);
            Controls.Add(lblPrompt);
            Controls.Add(txtInput);
            Controls.Add(btnCancel);
            Controls.Add(btnOK);
            Name = "CInputDialog";
            Text = "CInputDialog";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button btnOK;
        private Button btnCancel;
        private TextBox txtInput;
        private Label lblPrompt;
    }
}