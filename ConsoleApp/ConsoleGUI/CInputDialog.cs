using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ConsoleGUI
{
    public partial class CInputDialog : Form
    {
        public string InputText { get; private set; }

        public CInputDialog(string title, string prompt)
        {
            InitializeComponent(); // Initialize the form components.

            this.Text = title; // Set the title of the form.
            lblPrompt.Text = prompt; // Set the prompt message.
            btnOK.Click += (sender, e) => { this.DialogResult = DialogResult.OK; };
            btnCancel.Click += (sender, e) => { this.DialogResult = DialogResult.Cancel; };
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            // Save the input text when OK is clicked.
            InputText = txtInput.Text;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            // Close the dialog without doing anything.
            this.Close();
        }

        // Call this static method to create and show the input dialog.
        public static string Show(string title, string prompt)
        {
            using (CInputDialog inputDialog = new CInputDialog(title, prompt))
            {
                if (inputDialog.ShowDialog() == DialogResult.OK)
                {
                    return inputDialog.InputText;
                }
                else
                {
                    return null; // or string.Empty if you prefer
                }
            }
        }

        private void lblPrompt_Click(object sender, EventArgs e)
        {

        }

        private void txtInput_KeyDown(object sender, KeyEventArgs e)
        {
       
        }
    }
}
