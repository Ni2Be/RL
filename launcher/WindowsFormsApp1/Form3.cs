using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form3 : Form
    {
        Form2 ref2;
        int tdagent_number;
        public Form3()
        {
            InitializeComponent();
        }
        public Form3(Form2 ref2, int tdagent_number)
        {
            InitializeComponent();
            this.ref2 = ref2;
            this.tdagent_number = tdagent_number;
            numericUpDown1.Maximum = tdagent_number;
            textBox1.Text = ref2.tdName[0];
            if (ref2.tdMode[0] == 0) radioButton1.Checked = true;
            else radioButton2.Checked = true;

        }
        private void Form3_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void saveFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            textBox1.Text = ref2.tdName[(int)numericUpDown1.Value - 1];
            if (ref2.tdMode[(int)numericUpDown1.Value - 1] == 0) radioButton1.Checked = true;
            else radioButton2.Checked = true;

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            ref2.tdName[(int)numericUpDown1.Value - 1] = textBox1.Text;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            ref2.tdMode[(int)numericUpDown1.Value - 1] = 0;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            ref2.tdMode[(int)numericUpDown1.Value - 1] = 1;
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            
        }
    }
}
