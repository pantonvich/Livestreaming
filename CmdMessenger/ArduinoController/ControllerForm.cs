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
using WebSocket4Net;

//Websocket references: 
//https://github.com/programistka/WebSocketsDemo/blob/master/WebSocketsClientDemo/Client.cs
//https://github.com/Palakis/obs-websocket
//https://github.com/kerryjiang/WebSocket4Net


namespace ArduinoController
{
    public partial class ControllerForm : Form
    {
        private readonly ArduinoController _arduinoController;
        private double _ledFrequency;
        private WebSocket _websocketClient; //= new WebSocket("ws://127.0.0.1:4444");

        static public Color _connectedBackColor = Color.LightGreen;
        static public Color _disconnectedBackColor = Color.LightSalmon;
        public ControllerForm()
        {
            InitializeComponent();
            _arduinoController = new ArduinoController();
            _arduinoController.Setup(this);

            btnConnect.BackColor = _disconnectedBackColor;
            btnTitleSponsorSave.BackColor = _disconnectedBackColor;
            btnEventNameSave.BackColor = _disconnectedBackColor;
            btnWebSocket.BackColor = _disconnectedBackColor;

        }
        
        private bool _websocketClientOpen = false;
        public void OpenWebSocket()
        {
            if (_websocketClientOpen)
            {
                _websocketClient.Close();
            }
            else
            {
                //"ws://127.0.0.1:4444"
                //txtWebScocket
                _websocketClient = new WebSocket(txtWebScocketIp.Text);
                _websocketClient.Opened += new EventHandler(websocketClient_Opened);
                _websocketClient.Closed += new EventHandler(websocketClient_Closed);
                _websocketClient.MessageReceived += new EventHandler<MessageReceivedEventArgs>(websocketClient_MsgRecieved);
                _websocketClient.Error += new EventHandler<SuperSocket.ClientEngine.ErrorEventArgs>(websocketClient_Error);
                _websocketClient.Open();
            }
        }

        private void websocketClient_Opened(Object s, EventArgs e ) {

            _websocketClientOpen = true;
            _websocketClient.Send("{\"request-type\":\"GetVersion\",\"message-id\":\"startup\"}");
            _websocketClient.Send("{\"request-type\":\"GetSceneList\",\"message-id\":\"GetScenes\"}");
            _websocketClient.Send("{\"request-type\":\"GetCurrentProfile\",\"message-id\":\"GetProfile\"}");
            _websocketClient.Send("{\"request-type\":\"GetCurrentScene\",\"message-id\":\"GetCurScene\"}");
            
            btnWebSocket.BackColor = _connectedBackColor;
        }

        private void websocketClient_Closed(Object s, EventArgs e)
        {
            _websocketClientOpen = false;
            btnWebSocket.BackColor = _disconnectedBackColor;
        }
        //delegate void SetTextCallback(string text);

        string recTimecode;
        string streamTimeCode;

        int ct = 9;
        private void websocketClient_MsgRecieved(Object s, MessageReceivedEventArgs e)
        {
            recTimecode = findItemValue(e.Message, "rec-timecode");
            streamTimeCode = findItemValue(e.Message, "stream-timecode");
            if (ct-- < 0) return;
            //SetMsgText(recTimecode);
            //SetMsgText(streamTimeCode);
            SetMsgText(findItemValue(e.Message, "obs-studio-version"));
            SetMsgText(findItemValue(e.Message, "obs-websocket-version"));

            SetMsgText(e.Message);

            checkSerialIsConnected();
        }

        private string findItemValue(string msg, string key)
        {
            string v = string.Empty;
            int pos = msg.IndexOf(key);
            if (pos > -1)
            {
                var endPos = msg.IndexOf(",", pos);
                if (endPos == -1) endPos = msg.Length;
                v = "\"" + msg.Substring(pos, endPos - pos);
            }
            return v;
        }
        private void websocketClient_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {
            SetMsgText(e.Exception.GetType() + ": " + e.Exception.Message + Environment.NewLine + e.Exception.StackTrace);

            if (e.Exception.InnerException != null)
            {
                SetMsgText(e.Exception.InnerException.GetType().ToString());
            }

            return;
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

        delegate void SetTextCallback(string text);

        public void SetMsgText(string s)
        {
            if (s == string.Empty) return;

            if (this.textBoxMsg.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetMsgText);
                this.Invoke(d, new object[] { s });
            }
            else
            {
                if (textBoxMsg.Lines.Length > 50)
                {
                    textBoxMsg.Text = textBoxMsg.Text.Remove(0, textBoxMsg.GetFirstCharIndexFromLine(1));
                }
                textBoxMsg.AppendText(s + Environment.NewLine);
                textBoxMsg.ScrollToCaret();
            }

       

        }

        public string GetPjonIdFromTag(string tag)
        {
            var pjonIdFind = $"txt{tag[0]}Id";
            var c = Controls.Find(pjonIdFind, true);

            if (c == null || c.Count() == 0) return "";

            var pjonId = c[0].Text.Substring(0, 1);

            return pjonId;
        }

        public string GetTagFromPjonId(string pjonMsg)
        {

            for (var x = 1; x < 5; x++) {
                var f = GetPjonIdFromTag(x.ToString());
                if (f == "") continue;

                if (f[0] == pjonMsg[0]) return x.ToString();
            }

            return "";

        }
        public void SetCamMsgText(string pjonMsg)
        {
            //SetMsgText(s);
            var timeStamp = DateTime.Now.ToString(" HH:mm:ss");
            var tagId  = GetTagFromPjonId(pjonMsg);

            if (tagId == "") return;

            var ctlFind = $"lblCam0{tagId}";
            var ctl = Controls.Find(ctlFind, true);
            if (ctl == null) return;

            ctl[0].Text = $"{pjonMsg}{timeStamp}";

            var ptz = "";

            if (pjonMsg.Count() > 3)
            {
                if (pjonMsg[1] == 't') ptz = "Tlt";
                else if (pjonMsg[1] == 'p') ptz = "Pan";

                if (ptz != "")
                {
                    ctlFind = $"lbl{ptz}0{tagId}";
                    ctl = Controls.Find(ctlFind, true);
                    if (ctl == null) return;
                    ctl[0].Text = pjonMsg.Substring(2);
                }
            }

            //var frtChar = s[0];

            //if (frtChar == txt1Id.Text[0])
            //{
            //    lblCam01.Text = s + timeStamp;
            //} else if (frtChar == txt2Id.Text[0])
            //{
            //    lblCam02.Text = s + timeStamp;
            //} else if (frtChar == txt3Id.Text[0])
            //{
            //    lblCam03.Text = s + timeStamp;                
            //}
        }

        //net use /delete /y w: >nul
        //net use<driver letter> \\<servername>\<sharename> /user:<username> <password>

        //http://stackoverflow.com/questions/659013/accessing-a-shared-file-unc-from-a-remote-non-trusted-domain-with-credentials

        //http://stackoverflow.com/questions/1411606/get-unc-path-from-a-local-path-or-mapped-path
        //    Here's some C# code with a wrapper function LocalToUNC, which seems to work OK, though I haven't tested it extensively.

        //        [DllImport("mpr.dll")]
        //static extern int WNetGetUniversalNameA(
        //    string lpLocalPath, int dwInfoLevel, IntPtr lpBuffer, ref int lpBufferSize
        //);

        //    // I think max length for UNC is actually 32,767
        //    static string LocalToUNC(string localPath, int maxLen = 2000)
        //    {
        //        IntPtr lpBuff;

        //        // Allocate the memory
        //        try
        //        {
        //            lpBuff = Marshal.AllocHGlobal(maxLen);
        //        }
        //        catch (OutOfMemoryException)
        //        {
        //            return null;
        //        }

        //        try
        //        {
        //            int res = WNetGetUniversalNameA(localPath, 1, lpBuff, ref maxLen);

        //            if (res != 0)
        //                return null;

        //            // lpbuff is a structure, whose first element is a pointer to the UNC name (just going to be lpBuff + sizeof(int))
        //            return Marshal.PtrToStringAnsi(Marshal.ReadIntPtr(lpBuff));
        //        }
        //        catch (Exception)
        //        {
        //            return null;
        //        }
        //        finally
        //        {
        //            Marshal.FreeHGlobal(lpBuff);
        //        }
        //    }
        private string _lastLapOutput = "";
        private DateTime _lastLapTime = DateTime.Now;
        //public string LogPath = @"C:\nginx-rtmp\www";
        //public string LogPath = @"C:\Users\pantonvi\AppData\Roaming\Microsoft\Windows\Network Shortcuts\Downloads";
        //public string LogPath = @"C:\obs-dev";
        //public string lapFile = @"\laps.html";
        //public string LapLogFileName = @"\laps_{0}.log";

        public void SetLapStatusWriteFiles(string output)
        {
            //var output = arguments.ReadStringArg();
            if (_lastLapOutput == output) return;

            //string[] dirs = Directory.GetDirectories(LogPath);
            var logPath = txtPath.Text;
            if (logPath.Substring(logPath.Length - 1, 1) != @"\") {
                logPath += @"\";
            }
            var logFile = txtLogFile.Text;
            var lapFile = txtLapsToGoFile.Text;

            var lapTime = DateTime.Now;
            var dayLogName = lapTime.ToString("yyyyMMdd");
            var settingLap = ((lapTime - _lastLapTime).Seconds < 5 ? " *" : "");

            var text1 = string.Format("{0}{1}", (output == "-1" ? "" : output), Environment.NewLine);
            var text2 = string.Format("{0}{1}{2}", output, settingLap, Environment.NewLine);
            var line = $"\"lap\":\"{output}{settingLap}\", \"system-datetime\":\"{lapTime.ToString("s")}\", \"event-title\":\"{currentFeedEventNameText}\", {(streamTimeCode == ""? "\"\"":streamTimeCode)}, {(recTimecode == "" ? "\"\"" : recTimecode)}{Environment.NewLine}";

            _lastLapOutput = output;
            _lastLapTime = lapTime;

            SetLapsText(text2);
            try
            {
                using (var file = new StreamWriter(logPath + lapFile))
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
                File.AppendAllText(logPath + string.Format(logFile, dayLogName), line);
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

            var tag = (string)((Button)sender).Tag;

            var pjonId = GetPjonIdFromTag(tag);

            var ctlFind = $"cam{tag[0]}seconds";
            var ctl = Controls.Find(ctlFind, true);

            if (ctl == null) return;

            var sec = ctl[0].Text;

            if (sec == string.Empty) sec = "9"; 

            var msg = $"{pjonId}{tag.Substring(1)}{sec}";

            _arduinoController.SetCamMsg(msg);
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
            checkSerialIsConnected();
        }

        private void checkSerialIsConnected()
        {
            btnConnect.BackColor = (_arduinoController.IsConnected() ? _connectedBackColor : _disconnectedBackColor); 
        }
        private void radioVideoSource_CheckedChanged(object sender, EventArgs e)
        {
            var tag = (string)((RadioButton)sender).Tag;
            var pjonId = GetPjonIdFromTag(tag);

            var msg = pjonId + tag.Substring(1);

            //switch (tag[0])
            //{
            //    case '1':
            //        tag = txt1Id.Text.Substring(0, 1) + tag.Substring(1);
            //        break;
            //    case '2':
            //        tag = txt2Id.Text.Substring(0, 1) + tag.Substring(1);
            //        break;
            //    case '3':
            //        tag = txt3Id.Text.Substring(0, 1) + tag.Substring(1);
            //        break;
            //}
            _arduinoController.SetCamMsg(msg);

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

                var bufId = v/1000;
                var vNotId = (v - bufId * 1000).ToString();

                var tagId = GetTagFromPjonId(s);

                if (tagId == "") return;

                var ctlFind = $"bufCt0{tagId}";
                var ctl = Controls.Find(ctlFind, true);
                if (ctl == null) return;

                ctl[0].Text = vNotId;

                //var frtChar = s[0];
                //if (frtChar == txt1Id.Text[0])
                //{
                //    bufCt01.Text = vNotId;
                //}
                //else if (frtChar == txt2Id.Text[0])
                //{
                //    bufCt02.Text = vNotId;
                //}
                //else if (frtChar == txt3Id.Text[0])
                //{
                //    bufCt03.Text = vNotId;
                //}
            }
            else
            {
                bufCt.Text = s;
            }
        }

        private void bufCt_Click(object sender, EventArgs e)
        {
            var tag = (string)((Label)sender).Tag;

            var pjonId = GetPjonIdFromTag(tag); 
            
            var msg = pjonId + tag.Substring(1);
            //switch (tag[0])
            //{
            //    case '1':
            //        tag = txt1Id.Text.Substring(0, 1) + tag.Substring(1);
            //        break;
            //    case '2':
            //        tag = txt2Id.Text.Substring(0, 1) + tag.Substring(1);
            //        break;
            //    case '3':
            //        tag = txt3Id.Text.Substring(0, 1) + tag.Substring(1);
            //        break;
            //}
            _arduinoController.SetPjonRemoveAll(int.Parse(msg));
        }

        private void btnEventItemPrev_Click(object sender, EventArgs e)
        {
            eventItemPrevNext(-1);
        }

        private void btnEventItemNext_Click(object sender, EventArgs e)
        {
            eventItemPrevNext(1);
        }

        private void eventItemPrevNext(int direction)
        {
            var x = int.Parse(txtEventLineItem.Text) + direction;

            var path = txtPath.Text;
            if (path.Substring(path.Length - 1, 1) != @"\")
            {
                path += @"\";
            }

            var pathFile = path + txtEventNamesFile.Text;

            if (!File.Exists(pathFile))
            {
                SetMsgText($"File doesn't exist: {pathFile}!");
                return;
            }
            var fileStrings = File.ReadAllLines(pathFile);

            if (x >= fileStrings.Count()) x = fileStrings.Count() - 1;
            
            if (x < 0) x = 0;
            txtEventLineItem.Text = x.ToString();
            txtEventName.Text = fileStrings[x];
        }

        string currentFeedEventNameText;

        private void btnEventItemSave_Click(object sender, EventArgs e)
        {
            currentFeedEventNameText = txtEventName.Text;

            var path = txtPath.Text;
            if (path.Substring(path.Length - 1, 1) != @"\")
            {
                path += @"\";
            }

            var pathFile = path + txtEventNameFile.Text;

            File.WriteAllText(pathFile, txtEventName.Text);

            btnEventNameSave.BackColor = _connectedBackColor;

            var pos = txtEventName.Text.IndexOf("Cat.", 0, StringComparison.OrdinalIgnoreCase);

            if (pos != -1)
            {
                txtCat.Text = txtEventName.Text.Substring(pos + 5, 1);
                //if (chkShowResults.Checked)
            }
            else
            {
                if (txtEventName.Text.IndexOf("Women", 0, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    txtCat.Text = "W";
                }
                else
                {
                    txtCat.Text = "";
                }
            }


            if (chkUpdateLap.Checked)
            {
                //expect: lap in front of laps i.e. "ljsdlfjsdl 50 lap lsdjflks sprints every 10 laps"
                var posSkipLaps = txtEventName.Text.IndexOf(" laps", 0, StringComparison.OrdinalIgnoreCase) - 1;
                var posEnd = txtEventName.Text.IndexOf(" lap", 0, StringComparison.OrdinalIgnoreCase) - 1;

                if (posEnd != -1 && posSkipLaps != posEnd)
                {

                    if (posEnd > -1)
                    {
                        //accound for "56 lap" and not just "dslfjsdlkfj 50 lap"
                        var posStrt = 0;

                        if (posEnd > 2)
                        {
                            posStrt = txtEventName.Text.LastIndexOf(" ", posEnd);
                        }

                        if (posStrt > -1)
                        {
                            var lap = "";
                            if (posEnd == 0 && posStrt == 0)
                            {
                                lap = txtEventName.Text.Substring(0, 1);
                            }
                            else
                            {
                                lap = txtEventName.Text.Substring(posStrt + 1, posEnd - posStrt);
                            }

                            txtBoxLaps.Text = (int.Parse(lap) + 1).ToString();
                            txtLaps_Click(null, null);
                            return;
                        }
                    }
                }
            }
            txtBoxLaps.Text = "-1";
            txtLaps_Click(null, null);


        }

        private void btnTitleSponsorSave_Click(object sender, EventArgs e)
        {
            //currentFeedEventNameText = txtFeedEventName.Text;

            var path = txtPath.Text;
            if (path.Substring(path.Length - 1, 1) != @"\")
            {
                path += @"\";
            }

            var pathFile = path + txtTitleSponsorFile.Text;

            File.WriteAllText(pathFile, txtTitleSponsor.Text);

            btnTitleSponsorSave.BackColor = _connectedBackColor;
        }

        private void btnPos_Click(object sender, EventArgs e)
        {
            var tag = (string)((Button)sender).Tag;
            //var pjonIdFind = $"txt{tag[0]}Id";
            //var pjonId = Controls.Find(pjonIdFind, true)[0].Text.Substring(0, 1);
            var pjonId = GetPjonIdFromTag(tag);

            var find = $"txtPos{tag}";

            var tlt = Controls.Find(find + "t", true)[0].Text;
            var pos = Controls.Find(find + "p", true)[0].Text;

            if (tlt != "" && tlt != "0" && tlt !="999")
            _arduinoController.SetCamMsg($"{pjonId}t{tlt}");

            if (pos != "" && pos != "0" && pos != "999")
                _arduinoController.SetCamMsg($"{pjonId}p{pos}");

        }

        private void btnStp_Click(object sender, EventArgs e)
        {
            var tag = (string)((Button)sender).Tag;
            var pjonId = GetPjonIdFromTag(tag);

            _arduinoController.SetCamMsg($"{pjonId}s");

        }

        private void btnWebSocket_Click(object sender, EventArgs e)
        {
            OpenWebSocket();
        }

        private void txtFeedEventName_TextChanged(object sender, EventArgs e)
        {
            btnEventNameSave.BackColor = _disconnectedBackColor;
        }

        Timer stateTimer;

        private void txtCat_TextChanged(object sender, EventArgs e)
        {

            if (chkUpdateLap.Checked && _websocketClientOpen)
            {
                var resultCat = txtCat.Text.Trim();
                if (resultCat != "")
                {
                    //,\"scene-name\":\"Scene\"
                    _websocketClient.Send("{\"request-type\":\"SetSourceRender\",\"source\":\"Result"+ resultCat + "\",\"render\":true}");

                    stateTimer = new Timer();
                    stateTimer.Interval = 60000;
                    stateTimer.Tick += new EventHandler(Timer_Tick);
                    stateTimer.Start();

                }
            }
        }
        void Timer_Tick(Object myObject, EventArgs myEventArgs)
        {
            _websocketClient.Send("{\"request-type\":\"SetSourceRender\",\"source\":\"ResultA\",\"render\":false}");
            stateTimer.Stop();
        }

    }
}
