using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ArduinoController
{
    public partial class ControllerForm : Form
    {
        private readonly ArduinoController _arduinoController;
        private double _ledFrequency;

        public ControllerForm()
        {
            InitializeComponent();
            _arduinoController = new ArduinoController();
            _arduinoController.Setup(this);
        }

        // Update arduinoController on value checkbox checked/unchecked
        private void EnableLedCheckBoxCheckedChanged(object sender, EventArgs e)
        {
            _arduinoController.SetLedState(EnableLedCheckBox.Checked);
        }

        // Update value label and arduinoController on value changed using slider
        private void LedFrequencyTrackBarScroll(object sender, EventArgs e)
        {
            _ledFrequency = 0.4 + ((double)LedFrequencyLabelTrackBar.Value) / 25.0;
            LedFrequencyValue.Text = _ledFrequency.ToString(CultureInfo.InvariantCulture);
            _arduinoController.SetLedFrequency(_ledFrequency);
        }

        // Set ledState checkbox
        public void SetLedState(bool ledState)
        {
            EnableLedCheckBox.Checked = ledState;
        }

        public void SetMsgText(string s)
        {
            if (textBoxMsg.Lines.Length > 50)
            {
                textBoxMsg.Text = textBoxMsg.Text.Remove(0, textBoxMsg.GetFirstCharIndexFromLine(1));
            }
            textBoxMsg.AppendText(s + Environment.NewLine);
            textBoxMsg.ScrollToCaret();          

        }

        public void SetCamMsgText(string s)
        {
            //SetMsgText(s);
            var timeStamp = DateTime.Now.ToString(" HH:mm:ss");
            var frtChar = s[0];

            if (frtChar == txt1Id.Text[0])
            {
                lblCam01.Text = s + timeStamp;
            } else if (frtChar == txt2Id.Text[0])
            {
                lblCam02.Text = s + timeStamp;
            } else if (frtChar == txt3Id.Text[0])
            {
                lblCam03.Text = s + timeStamp;                
            }
        }

        private string _lastLapOutput = "";
        private DateTime _lastLapTime = DateTime.Now;
        //public string LogPath = @"C:\nginx-rtmp\www";
        //public string LogPath = @"C:\Users\pantonvi\AppData\Roaming\Microsoft\Windows\Network Shortcuts\Downloads";
        //public string LogPath = @"C:\obs-dev";
        public string LapFileName = @"\laps.html";
        public string LapLogFileName = @"\laps_{0}.log";

        public void SetLapStatusWriteFiles(string output)
        {
            //var output = arguments.ReadStringArg();
            if (_lastLapOutput == output) return;

            //string[] dirs = Directory.GetDirectories(LogPath);
            var logPath = txtLogPath.Text;
            var lapTime = DateTime.Now;
            var dayLogName = lapTime.ToString("yyyyMMdd");
            var settingLap = ((lapTime - _lastLapTime).Seconds < 5 ? " *" : "");

            var text1 = string.Format("{0}{1}", (output == "-1" ? "" : output), Environment.NewLine);
            var text2 = string.Format("{0}{1}{2}", output, settingLap, Environment.NewLine);
            var line = string.Format("{1} {0}", text2, lapTime.ToString("s"));
            _lastLapOutput = output;
            _lastLapTime = lapTime;

            SetLapsText(text2);
            try
            {
                using (var file = new StreamWriter(logPath + LapFileName))
                {
                    file.WriteLine(text1);
                }
                //File.WriteAllText(logPath + LapFileName, text1);
            }
            catch (Exception e)  //(IOException e)
            {
                SetMsgText(e.Message);
            }

            //log laps
            try
            {
                File.AppendAllText(logPath + string.Format(LapLogFileName, dayLogName), line);
            }
            catch (Exception e)
            {
                SetMsgText(e.Message);
            }
        }

        public void SetLapsText(string s)
        {
            txtLaps.Text = s;
        }
        // Set frequency slider
        public void SetFrequency(double ledFrequency)
        {
            LedFrequencyLabelTrackBar.Value = (int) ((ledFrequency - 0.4)*2.5);
        }

        // Update value label and arduinoController on value changed
        private void LedFrequencyLabelTrackBarValueChanged(object sender, EventArgs e)
        {
            LedFrequencyTrackBarScroll(sender,e);
        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing )
            {
                _arduinoController.Exit();
                if (components!=null)
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void ControllerForm_Load(object sender, EventArgs e)
        {

        }

        private void btnCam_Click(object sender, EventArgs e)
        {
            var tag = (string)((Button) sender).Tag;
            var sec = string.Empty;

            switch (tag[0])
            {
                case '1':
                    sec = cam1seconds.Text;
                    tag = txt1Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
                case '2':
                    sec = cam2seconds.Text;
                    tag = txt2Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
                case '3':
                    sec = cam3seconds.Text;
                    tag = txt3Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
            }

            if (sec == string.Empty) sec = "9"; 
            
            _arduinoController.SetCamMsg(tag + sec);
        }

        private void txtLaps_Click(object sender, EventArgs e)
        {
            //SetLapsText(txtBoxLaps.Text);
            var txtBoxLapsClean = (txtBoxLaps.Text.Replace("*", "")).Replace(" ", "");
            var txtLapsClean = (txtBoxLaps.Text.Replace("*", "")).Replace(" ", "");
            if (string.IsNullOrWhiteSpace(txtLapsClean)) txtLapsClean = "-1";
            if (!string.IsNullOrWhiteSpace(txtBoxLapsClean)
                && int.Parse(txtBoxLapsClean) == int.Parse(txtLapsClean))
            {
                //var lap = (chkUp.Checked ? 1 : -1);
                var lap = int.Parse(txtBoxLaps.Text) + (chkUp.Checked ? 1 : -1);
                if (lap < -1) lap = -1;
                txtBoxLaps.Text = lap.ToString();
                txtLaps.Text = lap.ToString();
            }

            if (chkManualOverride.Checked)
            {
                SetLapStatusWriteFiles(txtBoxLaps.Text);
            }
            else
            {
                _arduinoController.SetLapStatus(txtBoxLaps.Text);
            }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            _arduinoController.Connect(txtComPort.Text);
        }

        private void radioVideoSource_CheckedChanged(object sender, EventArgs e)
        {
            var tag = (string)((RadioButton)sender).Tag;
            switch (tag[0])
            {
                case '1':
                    tag = txt1Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
                case '2':
                    tag = txt2Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
                case '3':
                    tag = txt3Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
            }
            _arduinoController.SetCamMsg(tag);

        }

        private void label1_Click(object sender, EventArgs e)
        {
            _arduinoController.SetPjonRemoveAll();
        }

        public void SetPjonBufferCt(string s)
        {
            int v;
            if (!int.TryParse(s, out v)) return;
            if (v > 1000)
            {
                var frtChar = s[0];
                var tagId = v/1000;
                var vNotId = (v - tagId*1000).ToString();

                if (frtChar == txt1Id.Text[0])
                {
                    bufCt01.Text = vNotId;
                }
                else if (frtChar == txt2Id.Text[0])
                {
                    bufCt02.Text = vNotId;
                }
                else if (frtChar == txt3Id.Text[0])
                {
                    bufCt03.Text = vNotId;
                }
            }
            else
            {
                bufCt.Text = s;
            }
        }

        private void bufCt_Click(object sender, EventArgs e)
        {
            var tag = (string)((Label)sender).Tag;
            switch (tag[0])
            {
                case '1':
                    tag = txt1Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
                case '2':
                    tag = txt2Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
                case '3':
                    tag = txt3Id.Text.Substring(0, 1) + tag.Substring(1);
                    break;
            }
            _arduinoController.SetPjonRemoveAll(int.Parse(tag));
        }

        private void eventTitleLinePrev_Click(object sender, EventArgs e)
        {
            var x = int.Parse(eventTitleLineItem.Text) - 1;

            var fileStrings = File.ReadAllLines(eventTitlesPathFile.Text);

            if (x >= fileStrings.Count()) x = fileStrings.Count() - 1;
            if (x < 0) x = 0;
            eventTitleLineItem.Text = x.ToString();

            eventTitle.Text = fileStrings[x];

        }

        private void eventTitleLineNext_Click(object sender, EventArgs e)
        {
            var x = int.Parse(eventTitleLineItem.Text) + 1;

            var fileStrings = File.ReadAllLines(eventTitlesPathFile.Text);

            if (x >= fileStrings.Count()) x = fileStrings.Count() - 1;
            
            if (x < 0) x = 0;
            eventTitleLineItem.Text = x.ToString();
            eventTitle.Text = fileStrings[x];
        }

        private void eventTitleLineSave_Click(object sender, EventArgs e)
        {
            File.WriteAllText(eventTitlePathFile.Text,eventTitle.Text);
        }


    }
}
