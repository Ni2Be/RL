using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        
        public int humanPlayers = 1;
        public int MCTSAgents = 0;
        public int ReflexAgents = 0;
        public int RandomAgents = 0;
        public int TDAgents = 0;
        public int game = 0;
        public int console_output = 1;
        public String tdOptions = "TD_OPTIONS:";
        public int max_Score_Limit = 10000;
        public String gameName = "Snake";
        public Form1()
        {

            InitializeComponent();
            
            label2.Text = "Settings: Game: " + gameName + " - Humans: " + humanPlayers + " - KI Agents: " + (MCTSAgents + ReflexAgents + RandomAgents + TDAgents) + " - Score: " + max_Score_Limit;
            label3.Text = tdOptions;

        }

        private void button1_Click(object sender, EventArgs e)
        {
           
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // Create a new instance of the Form2 class
            Form2 settingsForm = new Form2(this);

            // Show the settings form
            settingsForm.Show();
            
        }

        private void Form1_Activated(object sender, EventArgs e)
        {
            label2.Text = "Settings: Game: " + gameName + " - Humans: " + humanPlayers + " - KI Agents: " + (MCTSAgents + ReflexAgents + RandomAgents + TDAgents) + " - Score: " + max_Score_Limit;
            label3.Text = tdOptions;
        }

        private void label2_Click(object sender, EventArgs e)
        {
            label2.Text = "Settings: Game: " + gameName + " - Humans: " + humanPlayers + " - KI Agents: " + (MCTSAgents + ReflexAgents + RandomAgents + TDAgents) + " - Score: " + max_Score_Limit;
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();
            if (game == 0)
                startInfo.FileName = @"Release\Snake.exe";
            else
                startInfo.FileName = @"Release\Pong.exe";
            startInfo.Arguments = @"" + humanPlayers + " " + MCTSAgents + " "+ " " + ReflexAgents + " "+ " " + RandomAgents + " " + TDAgents + " " + max_Score_Limit + " " + console_output;
            Process.Start(startInfo);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
