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
    public partial class Form2 : Form
    {
        Form1 ref1;
        public List<String> tdName = new List<String>();
        public List<int> tdMode = new List<int>();


        //Beschreibungen als Strings
        public String[] descriptions = new string[10];

        

        // "TD_OPTIONS:TRAIN,tdagent1:TRAIN,tdagent2";
        public Form2()
        {
            InitializeComponent();

        }
        public Form2(Form1 ref1)
        {
            InitializeComponent();
            this.ref1 = ref1;

            descriptions[0] = "Menschlicher Spieler: Ein menschlicher Spieler kann die Snake\n" +
                "oder das Paddle mit den Pfeiltasten steuern.";
            descriptions[1] = "MCTS Agent: Die MCTS ist eine statistische Baumsuche, die versucht\n" +
                "über simulierte Spiele (playouts) eine Vorstellung davon zu bekommen,\nwelche nächste Aktion die beste ist.\n" +
                "Der MCTS Agent erhält für Pong immer die Möglichkeit ein paar\nSpielzüge zu simulieren, dies führt evtl. zu verminderter Performance.";
            descriptions[2] = "Reflex Agent: Der Reflex Agent handelt solange zufällig,\n" +
                "bis er eine Wand, einen Apfel oder einen Ball sieht.\n" +
                "Dann versucht er auf das entsprechende Ziel mit einer Aktion zu reagieren.";
            descriptions[3] = "Random Agent: Der Random Agent handelt komplett zufällig.";
            descriptions[4] = "TD Agent: Der TD Agent handelt nach dem Temporal Difference Algorithmus.\n" +
                "Jede nächstmögliche Aktion erhält einen Wert. Je größer dieser Wert,\n" +
                "desto wahrscheinlicher ist die Ausführung dieser Aktion. \n" +
                "Der TD Agent kann trainiert werden und die Trainingsdaten\nfür weitere Spielesets angewendet werden.";

        }
        private void Form2_Load(object sender, EventArgs e)
        {

        }

        private void load_descriptions()
        {
            label7.Text = "";
            if (numericUpDown1.Value > 0)
            {
                label7.Text += descriptions[0]+"\n\n";
            }
            if (numericUpDown2.Value > 0)
            {
                label7.Text += descriptions[1] + "\n\n";
            }
            if (numericUpDown3.Value > 0)
            {
                label7.Text += descriptions[2] + "\n\n";
            }
            if (numericUpDown4.Value > 0)
            {
                label7.Text += descriptions[3] + "\n\n";
            }
            if (numericUpDown5.Value > 0)
            {
                label7.Text += descriptions[4];
            }
        }
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            numericUpDown1.Maximum = 1;
            numericUpDown2.Maximum = 4;
            numericUpDown3.Maximum = 4;
            numericUpDown4.Maximum = 4;
            numericUpDown5.Maximum = 4;

            numericUpDown1.Value = 1;
            numericUpDown2.Value = 0;
            numericUpDown3.Value = 0;
            numericUpDown4.Value = 0;
            numericUpDown5.Value = 0;

            label8.Text = "Anzahl der Leben:";

            load_descriptions();
            /*
            label1.Enabled = true;
            label2.Enabled = true;
            label3.Enabled = true;
            label4.Enabled = true;
            label5.Enabled = true;

            numericUpDown1.Enabled = true;
            numericUpDown2.Enabled = true;
            numericUpDown3.Enabled = true;
            numericUpDown4.Enabled = true;
            numericUpDown5.Enabled = true;
            */


        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }


        private void numericUpDown1_ValueChanged_1(object sender, EventArgs e)
        {
            load_descriptions();
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {

            load_descriptions();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int numericUpDownSum = (int) (numericUpDown1.Value + numericUpDown2.Value + numericUpDown3.Value + numericUpDown4.Value + numericUpDown5.Value);
            if (numericUpDownSum != 2 && radioButton2.Checked)
            {
                MessageBox.Show("Pong: Die Anzahl der Spieler darf nur 2 betragen!", "Hinweis");
            }
            else
            {
                ref1.humanPlayers = (int)numericUpDown1.Value;
                ref1.MCTSAgents = (int)numericUpDown2.Value;
                ref1.ReflexAgents = (int)numericUpDown3.Value;
                ref1.RandomAgents = (int)numericUpDown4.Value;
                ref1.TDAgents = (int)numericUpDown5.Value;
                ref1.max_Score_Limit = (int)numericUpDown6.Value;
                if (radioButton1.Checked)
                {
                    ref1.game = 0;
                    ref1.gameName = "Snake";
                }
                if (radioButton2.Checked)
                {
                    ref1.game = 1;
                    ref1.gameName = "Pong";
                }

                if (checkBox1.Checked) ref1.console_output = 1; else ref1.console_output = 0;

                ref1.tdOptions = "TD_OPTIONS";
                if (tdName.Count == 0)
                {
                    ref1.tdOptions += ":";
                }
                for (int i = 0; i < tdName.Count; i++)
                {
                    ref1.tdOptions += ":"+tdName[i] + "," + tdMode[i];

                }
                ref1.Refresh();
                this.Close();
            }
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            numericUpDown1.Maximum = 1;
            numericUpDown2.Maximum = 2;
            numericUpDown3.Maximum = 2;
            numericUpDown4.Maximum = 2;
            numericUpDown5.Maximum = 2;

            numericUpDown1.Value = 1;
            numericUpDown2.Value = 0;
            numericUpDown3.Value = 0;
            numericUpDown4.Value = 0;
            numericUpDown5.Value = 1;

            label8.Text = "Punktzahl bis Spielende:";

            load_descriptions();
            /*
            label1.Enabled = true;
            label2.Enabled = false;
            label3.Enabled = true;
            label4.Enabled = true;
            label5.Enabled = true;

            numericUpDown1.Enabled = true;
            numericUpDown2.Enabled = false;
            numericUpDown3.Enabled = true;
            numericUpDown4.Enabled = true;
            numericUpDown5.Enabled = true;
            */
        }

        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {
            load_descriptions();
        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            // Create a new instance of the Form3 class
            Form3 tdSettingsForm = new Form3(this, (int)numericUpDown5.Value);

            // Show the settings form
            tdSettingsForm.Show();
        }

        private void numericUpDown5_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDown5.Value > 0)
            {
                button2.Enabled = true;
            }
            else
            {
                button2.Enabled = false;
            }
            tdName.Clear();
            tdMode.Clear();
            for (int i = 0; i < numericUpDown5.Value; i++)
            {
                tdName.Add("tdagent"+(i+1));
                tdMode.Add(0);
            }
            load_descriptions();
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            load_descriptions();
        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }
    }
    }
