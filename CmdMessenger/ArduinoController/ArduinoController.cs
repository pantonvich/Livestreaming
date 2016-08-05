// *** ArduinoController ***

// This example expands the SendandReceiveArguments example. The PC will now sends commands to the Arduino when the trackbar 
// is pulled. Every TrackBarChanged events will queue a message to the Arduino to set the blink speed of the 
// internal / pin 13 LED
// 
// This example shows how to :
// - use in combination with WinForms
// - use in combination with ZedGraph
// - send queued commands
// - Use the CollapseCommandStrategy

using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;
using CommandMessenger;
using CommandMessenger.Queue;
using CommandMessenger.Transport.Serial;

namespace ArduinoController
{
    enum Command
    {
        Acknowledge,            // Command to acknowledge a received command
        Error,                  // Command to message that an error has occurred
        SetLed,                 // Command to turn led ON or OFF
        SetLedFrequency,        // Command to set led blink frequency
        LapStatus,              // Command to message Lap Counter
        SetCamMsg,              // Command to send message to cam
        BtnStatus,
        PjonRemoveAll, //reset the pjon buffer
        PjonBufferCt,  //number of unsent messages in buffer - if more that 1000 than includes Id
    };

    public class ArduinoController
    {
        // This class (kind of) contains presentation logic, and domain model.
        // ChartForm.cs contains the view components 

        private SerialTransport   _serialTransport;
        private CmdMessenger      _cmdMessenger;
        private ControllerForm    _controllerForm;

        // ------------------ MAIN  ----------------------
        private string _comPort = "COM15";
        // Setup function
        public void Connect(string comPort)
        {
            _comPort = comPort;
            Exit();
            Setup(_controllerForm);
        }
        public void Setup(ControllerForm controllerForm)
        {
            // storing the controller form for later reference
            _controllerForm = controllerForm;

            // Create Serial Port object
            // Note that for some boards (e.g. Sparkfun Pro Micro) DtrEnable may need to be true.
            _serialTransport = new SerialTransport
            {
                CurrentSerialSettings = { PortName = _comPort, BaudRate = 115200, DtrEnable = false } // object initializer
            };

            // Initialize the command messenger with the Serial Port transport layer
            // Set if it is communicating with a 16- or 32-bit Arduino board
            _cmdMessenger = new CmdMessenger(_serialTransport, BoardType.Bit16);

            // Tell CmdMessenger to "Invoke" commands on the thread running the WinForms UI
            _cmdMessenger.ControlToInvokeOn = _controllerForm;

            // Attach the callbacks to the Command Messenger
            AttachCommandCallBacks();

            // Attach to NewLinesReceived for logging purposes
            _cmdMessenger.NewLineReceived += NewLineReceived;

            // Attach to NewLineSent for logging purposes
            _cmdMessenger.NewLineSent += NewLineSent;                       

            // Start listening
            _cmdMessenger.Connect();

            _controllerForm.SetLedState(true);
            _controllerForm.SetFrequency(2);

            var status = _cmdMessenger.Connect();

            if (status) return;

            const string s = @"No connection ";
            Console.WriteLine(s + _comPort);
            _controllerForm.SetMsgText(s + _comPort);
        }

        // Exit function
        public void Exit()
        {
            if (_cmdMessenger != null)
            {
                // Stop listening
                _cmdMessenger.Disconnect();

                // Dispose Command Messenger
                _cmdMessenger.Dispose();
            }
            // Dispose Serial Port object
            // Dispose Serial Port object
            if (_serialTransport != null) _serialTransport.Dispose();  
        }

        /// Attach command call backs. 
        private void AttachCommandCallBacks()
        {
            _cmdMessenger.Attach(OnUnknownCommand);
            _cmdMessenger.Attach((int)Command.Acknowledge, OnAcknowledge);
            _cmdMessenger.Attach((int)Command.Error, OnError);
            _cmdMessenger.Attach((int)Command.LapStatus, OnLapStatus);
            _cmdMessenger.Attach((int)Command.SetCamMsg, OnCamMsg);
            _cmdMessenger.Attach((int)Command.BtnStatus, OnBtnStatus);
            _cmdMessenger.Attach((int)Command.PjonBufferCt, OnPjonBufferCt);
        }

        // ------------------  CALLBACKS ---------------------

        // Called when a received command has no attached function.
        // In a WinForm application, console output gets routed to the output panel of your IDE
        void OnUnknownCommand(ReceivedCommand arguments)
        {
            const string s = @"Command without attached callback received";
            Console.WriteLine(s);
            _controllerForm.SetMsgText(s);
        }

        // Callback function that prints that the Arduino has acknowledged
        void OnAcknowledge(ReceivedCommand arguments)
        {
            var output = arguments.ReadStringArg();
            if (string.IsNullOrWhiteSpace(output)) return;
            _controllerForm.SetCamMsgText(output);

            const string s = @" ACK ";
            Console.WriteLine(s);
            _controllerForm.SetMsgText(s + output);
        }

        // Callback function that prints that the Arduino has experienced an error
        void OnError(ReceivedCommand arguments)
        {
            const string s = @"Error ";
            Console.WriteLine(s + arguments.RawString);
            _controllerForm.SetMsgText(s + arguments.RawString);
        }

        // Log received line to console
        private void NewLineReceived(object sender, CommandEventArgs e)
        {
            var timeStamp = DateTime.Now.ToString("HH:mm:ss");
            const string s =  @" >> ";
            //Console.WriteLine(s + e.Command.CommandString());
            _controllerForm.SetMsgText(timeStamp + s + e.Command.CommandString());
        }

        // Log sent line to console
        private void NewLineSent(object sender, CommandEventArgs e)
        {
            const string s = @"<< ";
            Console.WriteLine(s + e.Command.CommandString());
            _controllerForm.SetMsgText(s + e.Command.CommandString());
        }

        // Sent command to change led blinking frequency
        public void SetLedFrequency(double ledFrequency)
        {
            // Create command to start sending data
            var command = new SendCommand((int)Command.SetLedFrequency,ledFrequency);

            // Put the command on the queue and wrap it in a collapse command strategy
            // This strategy will avoid duplicates of this certain command on the queue: if a SetLedFrequency command is
            // already on the queue when a new one is added, it will be replaced at its current queue-position. 
            // Otherwise the command will be added to the back of the queue. 
            // 
            // This will make sure that when the slider raises a lot of events that each send a new blink frequency, the 
            // embedded controller will not start lagging.
            _cmdMessenger.QueueCommand(new CollapseCommandStrategy(command));
        }


        // Sent command to change led on/of state
        public void SetLedState(bool ledState)
        {
            // Create command to start sending data
            //var command = new SendCommand((int)Command.SetLed, ledState);

            // Send command
            _cmdMessenger.SendCommand(new SendCommand((int)Command.SetLed, ledState));         
        }

        // Sent command to change led on/of state
        public void SetCamMsg(string camMsg)
        {
            // Send command
            _cmdMessenger.SendCommand(new SendCommand((int)Command.SetCamMsg, camMsg));
        }

        //sent zero to remove them all
        public void SetPjonRemoveAll(int deviceId = 0)
        {
            //sent zero to remove them all
            // Send command
            _cmdMessenger.SendCommand(new SendCommand((int)Command.PjonRemoveAll, deviceId));
        }

        private void OnCamMsg(ReceivedCommand arguments)
        {
            var output = arguments.ReadStringArg();
            if (string.IsNullOrWhiteSpace(output)) return;
           _controllerForm.SetCamMsgText(output);
        }

        private void OnPjonBufferCt(ReceivedCommand arguments)
        {
            var output = arguments.ReadStringArg();
            if (string.IsNullOrWhiteSpace(output)) return;
            _controllerForm.SetPjonBufferCt(output);
        }

             // Callback function that prints the Arduino status to the console
        private void OnLapStatus(ReceivedCommand arguments)
        {
            var output = arguments.ReadStringArg();
            _controllerForm.SetLapStatusWriteFiles(output);
        }

        /*
        private string _lastLapOutput = "";
        private DateTime _lastLapTime = DateTime.Now;
        //public string LogPath = @"C:\nginx-rtmp\www";
        //public string LogPath = @"C:\Users\pantonvi\AppData\Roaming\Microsoft\Windows\Network Shortcuts\Downloads";
        //public string LogPath = @"C:\obs-dev";
        public string LapFileName = @"\laps.html";
        public string LapLogFileName = @"\laps_{0}.log";

        // Callback function that prints the Arduino status to the console
        private void OnLapStatus(ReceivedCommand arguments)
        {
            var output = arguments.ReadStringArg();
            if (_lastLapOutput == output) return;

            //string[] dirs = Directory.GetDirectories(LogPath);

            var lapTime = DateTime.Now;
            var dayLogName = lapTime.ToString("yyyyMMdd");
            var settingLap = (( lapTime-_lastLapTime).Seconds<5?" *":"");
            var text1 = string.Format("{0}{1}", output, Environment.NewLine);
            var text2 = string.Format("{0}{1}{2}", output, settingLap, Environment.NewLine);
            var line = string.Format("{1} {0}", text2, lapTime.ToString("s"));

            _controllerForm.SetLapsText(text2);

            var logPath = _controllerForm.txtLogPath.Text;

            File.WriteAllText(logPath + LapFileName , text1);
            
            //log laps
            File.AppendAllText(logPath + string.Format(LapLogFileName, dayLogName), line);

            _lastLapOutput = output;
            _lastLapTime = lapTime;
        }
        */

        //private string _lastBtnOutput = "";
        //private DateTime _lastBtnTime = DateTime.Now;
        private void OnBtnStatus(ReceivedCommand arguments)
        {
            var output = arguments.ReadStringArg();
            //var btnTime = DateTime.Now;

            //if (_lastBtnOutput == output 
            //    && btnTime.Subtract(_lastBtnTime).Seconds < 1) return;

            int v;
            if(! int.TryParse(output,out v )) return;

            var in01 = int.Parse(_controllerForm.btn01In.Text);
            var in02 = int.Parse(_controllerForm.btn02In.Text);
            var in03 = int.Parse(_controllerForm.btn03In.Text);

            var out01 = int.Parse(_controllerForm.btn01Out.Text);
            var out02 = int.Parse(_controllerForm.btn02Out.Text);
            var out03 = int.Parse(_controllerForm.btn03Out.Text);

            //_controllerForm.SetLapsText(text2);
            if ((v & in01) == in01 && (v & out01) == out01)
                _controllerForm.btnCam01center.PerformClick();
            else if ((v & in01) == in01)
                _controllerForm.btnCam01in.PerformClick();
            else if ((v & out01) == out01)
                _controllerForm.btnCam01out.PerformClick();
            if ((v & in02) == in02 && (v & out02) == out02)
                _controllerForm.btnCam02center.PerformClick();
            else if ((v & in02) == in02)
                _controllerForm.btnCam02in.PerformClick();
            else if ((v & out02) == out02)
                _controllerForm.btnCam02out.PerformClick();
            if ((v & in03) == in03 && (v & out03) == out03)
                _controllerForm.btnCam03center.PerformClick();
            else if ((v & in03) == in03)
                _controllerForm.btnCam03in.PerformClick();
            else if ((v & out03) == out03)
                _controllerForm.btnCam03out.PerformClick();

            //_lastBtnOutput = output;
            //_lastBtnTime = btnTime;
        }

        internal void SetLapStatus(string laps)
        {
            _cmdMessenger.SendCommand(new SendCommand((int)Command.LapStatus, laps));
        }
    }
}
