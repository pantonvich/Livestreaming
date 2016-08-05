namespace ArduinoController
{
    partial class ControllerForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;



        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.EnableLedCheckBox = new System.Windows.Forms.CheckBox();
            this.LedFrequencyLabelTrackBar = new System.Windows.Forms.TrackBar();
            this.LedFrequencyLabel = new System.Windows.Forms.Label();
            this.LedFrequencyValue = new System.Windows.Forms.Label();
            this.btnCam01in = new System.Windows.Forms.Button();
            this.btnCam01center = new System.Windows.Forms.Button();
            this.btnCam01out = new System.Windows.Forms.Button();
            this.cam1seconds = new System.Windows.Forms.TextBox();
            this.cam1set = new System.Windows.Forms.Button();
            this.txtLaps = new System.Windows.Forms.Label();
            this.txtComPort = new System.Windows.Forms.TextBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.textBoxMsg = new System.Windows.Forms.TextBox();
            this.radioButton1_1 = new System.Windows.Forms.RadioButton();
            this.radioButton1_2 = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btn01Out = new System.Windows.Forms.TextBox();
            this.btn01In = new System.Windows.Forms.TextBox();
            this.bufCt01 = new System.Windows.Forms.Label();
            this.txt1Id = new System.Windows.Forms.TextBox();
            this.lblCam01 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btn02Out = new System.Windows.Forms.TextBox();
            this.btn02In = new System.Windows.Forms.TextBox();
            this.bufCt02 = new System.Windows.Forms.Label();
            this.txt2Id = new System.Windows.Forms.TextBox();
            this.lblCam02 = new System.Windows.Forms.Label();
            this.btnCam02in = new System.Windows.Forms.Button();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.btnCam02center = new System.Windows.Forms.Button();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.btnCam02out = new System.Windows.Forms.Button();
            this.cam2seconds = new System.Windows.Forms.TextBox();
            this.cam2set = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btn03Out = new System.Windows.Forms.TextBox();
            this.btn03In = new System.Windows.Forms.TextBox();
            this.txt3Id = new System.Windows.Forms.TextBox();
            this.lblCam03 = new System.Windows.Forms.Label();
            this.btnCam03in = new System.Windows.Forms.Button();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.btnCam03center = new System.Windows.Forms.Button();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.btnCam03out = new System.Windows.Forms.Button();
            this.cam3seconds = new System.Windows.Forms.TextBox();
            this.cam3set = new System.Windows.Forms.Button();
            this.bufCt03 = new System.Windows.Forms.Label();
            this.bufCt = new System.Windows.Forms.Label();
            this.txtBoxLaps = new System.Windows.Forms.TextBox();
            this.txtLogPath = new System.Windows.Forms.TextBox();
            this.chkManualOverride = new System.Windows.Forms.CheckBox();
            this.chkUp = new System.Windows.Forms.CheckBox();
            this.eventTitlesPathFile = new System.Windows.Forms.TextBox();
            this.eventTitle = new System.Windows.Forms.TextBox();
            this.eventTitleLineItem = new System.Windows.Forms.TextBox();
            this.eventTitleLinePrev = new System.Windows.Forms.Button();
            this.eventTitleLineNext = new System.Windows.Forms.Button();
            this.eventTitleLineSave = new System.Windows.Forms.Button();
            this.eventTitlePathFile = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.LedFrequencyLabelTrackBar)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // EnableLedCheckBox
            // 
            this.EnableLedCheckBox.AutoSize = true;
            this.EnableLedCheckBox.Checked = true;
            this.EnableLedCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.EnableLedCheckBox.Location = new System.Drawing.Point(174, 10);
            this.EnableLedCheckBox.Name = "EnableLedCheckBox";
            this.EnableLedCheckBox.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.EnableLedCheckBox.Size = new System.Drawing.Size(80, 17);
            this.EnableLedCheckBox.TabIndex = 0;
            this.EnableLedCheckBox.Text = "Enable Led";
            this.EnableLedCheckBox.UseVisualStyleBackColor = true;
            this.EnableLedCheckBox.Click += new System.EventHandler(this.EnableLedCheckBoxCheckedChanged);
            // 
            // LedFrequencyLabelTrackBar
            // 
            this.LedFrequencyLabelTrackBar.Location = new System.Drawing.Point(234, 33);
            this.LedFrequencyLabelTrackBar.Maximum = 240;
            this.LedFrequencyLabelTrackBar.Name = "LedFrequencyLabelTrackBar";
            this.LedFrequencyLabelTrackBar.Size = new System.Drawing.Size(208, 45);
            this.LedFrequencyLabelTrackBar.TabIndex = 1;
            this.LedFrequencyLabelTrackBar.Tag = "";
            this.LedFrequencyLabelTrackBar.TickFrequency = 10;
            this.LedFrequencyLabelTrackBar.Scroll += new System.EventHandler(this.LedFrequencyTrackBarScroll);
            this.LedFrequencyLabelTrackBar.ValueChanged += new System.EventHandler(this.LedFrequencyLabelTrackBarValueChanged);
            // 
            // LedFrequencyLabel
            // 
            this.LedFrequencyLabel.AutoSize = true;
            this.LedFrequencyLabel.Location = new System.Drawing.Point(158, 34);
            this.LedFrequencyLabel.Name = "LedFrequencyLabel";
            this.LedFrequencyLabel.Size = new System.Drawing.Size(78, 13);
            this.LedFrequencyLabel.TabIndex = 2;
            this.LedFrequencyLabel.Text = "Led Frequency";
            // 
            // LedFrequencyValue
            // 
            this.LedFrequencyValue.AutoSize = true;
            this.LedFrequencyValue.Location = new System.Drawing.Point(448, 36);
            this.LedFrequencyValue.Name = "LedFrequencyValue";
            this.LedFrequencyValue.Size = new System.Drawing.Size(13, 13);
            this.LedFrequencyValue.TabIndex = 3;
            this.LedFrequencyValue.Text = "0";
            // 
            // btnCam01in
            // 
            this.btnCam01in.Location = new System.Drawing.Point(24, 19);
            this.btnCam01in.Name = "btnCam01in";
            this.btnCam01in.Size = new System.Drawing.Size(31, 23);
            this.btnCam01in.TabIndex = 4;
            this.btnCam01in.Tag = "1i";
            this.btnCam01in.Text = "In";
            this.btnCam01in.UseVisualStyleBackColor = true;
            this.btnCam01in.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // btnCam01center
            // 
            this.btnCam01center.Location = new System.Drawing.Point(61, 19);
            this.btnCam01center.Name = "btnCam01center";
            this.btnCam01center.Size = new System.Drawing.Size(31, 23);
            this.btnCam01center.TabIndex = 5;
            this.btnCam01center.Tag = "1c";
            this.btnCam01center.Text = "Cn";
            this.btnCam01center.UseVisualStyleBackColor = true;
            this.btnCam01center.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // btnCam01out
            // 
            this.btnCam01out.Location = new System.Drawing.Point(99, 19);
            this.btnCam01out.Name = "btnCam01out";
            this.btnCam01out.Size = new System.Drawing.Size(31, 23);
            this.btnCam01out.TabIndex = 6;
            this.btnCam01out.Tag = "1o";
            this.btnCam01out.Text = "Ou";
            this.btnCam01out.UseVisualStyleBackColor = true;
            this.btnCam01out.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // cam1seconds
            // 
            this.cam1seconds.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cam1seconds.Location = new System.Drawing.Point(136, 19);
            this.cam1seconds.Name = "cam1seconds";
            this.cam1seconds.Size = new System.Drawing.Size(48, 23);
            this.cam1seconds.TabIndex = 7;
            this.cam1seconds.Text = "5";
            // 
            // cam1set
            // 
            this.cam1set.Location = new System.Drawing.Point(196, 19);
            this.cam1set.Name = "cam1set";
            this.cam1set.Size = new System.Drawing.Size(60, 23);
            this.cam1set.TabIndex = 15;
            this.cam1set.Tag = "1";
            this.cam1set.Text = "Config";
            this.cam1set.UseVisualStyleBackColor = true;
            this.cam1set.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // txtLaps
            // 
            this.txtLaps.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.txtLaps.Font = new System.Drawing.Font("Microsoft Sans Serif", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtLaps.Location = new System.Drawing.Point(476, 3);
            this.txtLaps.Name = "txtLaps";
            this.txtLaps.Size = new System.Drawing.Size(146, 56);
            this.txtLaps.TabIndex = 17;
            this.txtLaps.Text = "-1";
            this.txtLaps.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this.txtLaps.Click += new System.EventHandler(this.txtLaps_Click);
            // 
            // txtComPort
            // 
            this.txtComPort.Location = new System.Drawing.Point(288, 8);
            this.txtComPort.Name = "txtComPort";
            this.txtComPort.Size = new System.Drawing.Size(48, 20);
            this.txtComPort.TabIndex = 19;
            this.txtComPort.Text = "COM16";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(342, 6);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 20;
            this.btnConnect.Tag = "1";
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // textBoxMsg
            // 
            this.textBoxMsg.Location = new System.Drawing.Point(2, 3);
            this.textBoxMsg.Multiline = true;
            this.textBoxMsg.Name = "textBoxMsg";
            this.textBoxMsg.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxMsg.Size = new System.Drawing.Size(149, 329);
            this.textBoxMsg.TabIndex = 23;
            // 
            // radioButton1_1
            // 
            this.radioButton1_1.AutoSize = true;
            this.radioButton1_1.Location = new System.Drawing.Point(278, 25);
            this.radioButton1_1.Name = "radioButton1_1";
            this.radioButton1_1.Size = new System.Drawing.Size(31, 17);
            this.radioButton1_1.TabIndex = 24;
            this.radioButton1_1.TabStop = true;
            this.radioButton1_1.Tag = "1V1";
            this.radioButton1_1.Text = "1";
            this.radioButton1_1.UseVisualStyleBackColor = true;
            this.radioButton1_1.Click += new System.EventHandler(this.radioVideoSource_CheckedChanged);
            // 
            // radioButton1_2
            // 
            this.radioButton1_2.AutoSize = true;
            this.radioButton1_2.Location = new System.Drawing.Point(315, 25);
            this.radioButton1_2.Name = "radioButton1_2";
            this.radioButton1_2.Size = new System.Drawing.Size(31, 17);
            this.radioButton1_2.TabIndex = 25;
            this.radioButton1_2.TabStop = true;
            this.radioButton1_2.Tag = "1V2";
            this.radioButton1_2.Text = "2";
            this.radioButton1_2.UseVisualStyleBackColor = true;
            this.radioButton1_2.Click += new System.EventHandler(this.radioVideoSource_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btn01Out);
            this.groupBox1.Controls.Add(this.btn01In);
            this.groupBox1.Controls.Add(this.bufCt01);
            this.groupBox1.Controls.Add(this.txt1Id);
            this.groupBox1.Controls.Add(this.lblCam01);
            this.groupBox1.Controls.Add(this.btnCam01in);
            this.groupBox1.Controls.Add(this.radioButton1_2);
            this.groupBox1.Controls.Add(this.btnCam01center);
            this.groupBox1.Controls.Add(this.radioButton1_1);
            this.groupBox1.Controls.Add(this.btnCam01out);
            this.groupBox1.Controls.Add(this.cam1seconds);
            this.groupBox1.Controls.Add(this.cam1set);
            this.groupBox1.Location = new System.Drawing.Point(161, 72);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(678, 56);
            this.groupBox1.TabIndex = 26;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Turn One";
            // 
            // btn01Out
            // 
            this.btn01Out.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn01Out.Location = new System.Drawing.Point(550, 19);
            this.btn01Out.Name = "btn01Out";
            this.btn01Out.Size = new System.Drawing.Size(25, 23);
            this.btn01Out.TabIndex = 33;
            this.btn01Out.Tag = "1";
            this.btn01Out.Text = "4";
            this.btn01Out.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btn01In
            // 
            this.btn01In.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn01In.Location = new System.Drawing.Point(519, 19);
            this.btn01In.Name = "btn01In";
            this.btn01In.Size = new System.Drawing.Size(25, 23);
            this.btn01In.TabIndex = 32;
            this.btn01In.Tag = "1";
            this.btn01In.Text = "2";
            this.btn01In.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // bufCt01
            // 
            this.bufCt01.AutoSize = true;
            this.bufCt01.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bufCt01.ForeColor = System.Drawing.Color.Firebrick;
            this.bufCt01.Location = new System.Drawing.Point(637, 12);
            this.bufCt01.Name = "bufCt01";
            this.bufCt01.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.bufCt01.Size = new System.Drawing.Size(26, 29);
            this.bufCt01.TabIndex = 31;
            this.bufCt01.Tag = "1";
            this.bufCt01.Text = "0";
            this.bufCt01.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            this.bufCt01.Click += new System.EventHandler(this.bufCt_Click);
            // 
            // txt1Id
            // 
            this.txt1Id.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txt1Id.Location = new System.Drawing.Point(605, 19);
            this.txt1Id.Name = "txt1Id";
            this.txt1Id.Size = new System.Drawing.Size(25, 23);
            this.txt1Id.TabIndex = 27;
            this.txt1Id.Tag = "1";
            this.txt1Id.Text = "4";
            this.txt1Id.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblCam01
            // 
            this.lblCam01.AutoSize = true;
            this.lblCam01.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCam01.Location = new System.Drawing.Point(352, 21);
            this.lblCam01.Name = "lblCam01";
            this.lblCam01.Size = new System.Drawing.Size(47, 20);
            this.lblCam01.TabIndex = 26;
            this.lblCam01.Text = "None";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btn02Out);
            this.groupBox2.Controls.Add(this.btn02In);
            this.groupBox2.Controls.Add(this.bufCt02);
            this.groupBox2.Controls.Add(this.txt2Id);
            this.groupBox2.Controls.Add(this.lblCam02);
            this.groupBox2.Controls.Add(this.btnCam02in);
            this.groupBox2.Controls.Add(this.radioButton1);
            this.groupBox2.Controls.Add(this.btnCam02center);
            this.groupBox2.Controls.Add(this.radioButton2);
            this.groupBox2.Controls.Add(this.btnCam02out);
            this.groupBox2.Controls.Add(this.cam2seconds);
            this.groupBox2.Controls.Add(this.cam2set);
            this.groupBox2.Location = new System.Drawing.Point(161, 134);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(678, 56);
            this.groupBox2.TabIndex = 27;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Turn Three";
            // 
            // btn02Out
            // 
            this.btn02Out.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn02Out.Location = new System.Drawing.Point(550, 22);
            this.btn02Out.Name = "btn02Out";
            this.btn02Out.Size = new System.Drawing.Size(25, 23);
            this.btn02Out.TabIndex = 35;
            this.btn02Out.Tag = "1";
            this.btn02Out.Text = "16";
            this.btn02Out.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btn02In
            // 
            this.btn02In.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn02In.Location = new System.Drawing.Point(519, 22);
            this.btn02In.Name = "btn02In";
            this.btn02In.Size = new System.Drawing.Size(25, 23);
            this.btn02In.TabIndex = 34;
            this.btn02In.Tag = "1";
            this.btn02In.Text = "8";
            this.btn02In.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // bufCt02
            // 
            this.bufCt02.AutoSize = true;
            this.bufCt02.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bufCt02.ForeColor = System.Drawing.Color.Firebrick;
            this.bufCt02.Location = new System.Drawing.Point(637, 16);
            this.bufCt02.Name = "bufCt02";
            this.bufCt02.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.bufCt02.Size = new System.Drawing.Size(26, 29);
            this.bufCt02.TabIndex = 32;
            this.bufCt02.Tag = "2";
            this.bufCt02.Text = "0";
            this.bufCt02.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            this.bufCt02.Click += new System.EventHandler(this.bufCt_Click);
            // 
            // txt2Id
            // 
            this.txt2Id.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txt2Id.Location = new System.Drawing.Point(605, 22);
            this.txt2Id.Name = "txt2Id";
            this.txt2Id.Size = new System.Drawing.Size(25, 23);
            this.txt2Id.TabIndex = 28;
            this.txt2Id.Tag = "2";
            this.txt2Id.Text = "2";
            this.txt2Id.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblCam02
            // 
            this.lblCam02.AutoSize = true;
            this.lblCam02.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCam02.Location = new System.Drawing.Point(352, 22);
            this.lblCam02.Name = "lblCam02";
            this.lblCam02.Size = new System.Drawing.Size(47, 20);
            this.lblCam02.TabIndex = 27;
            this.lblCam02.Text = "None";
            // 
            // btnCam02in
            // 
            this.btnCam02in.Location = new System.Drawing.Point(24, 22);
            this.btnCam02in.Name = "btnCam02in";
            this.btnCam02in.Size = new System.Drawing.Size(31, 23);
            this.btnCam02in.TabIndex = 4;
            this.btnCam02in.Tag = "2i";
            this.btnCam02in.Text = "In";
            this.btnCam02in.UseVisualStyleBackColor = true;
            this.btnCam02in.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(315, 28);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(31, 17);
            this.radioButton1.TabIndex = 25;
            this.radioButton1.TabStop = true;
            this.radioButton1.Tag = "2V2";
            this.radioButton1.Text = "2";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.Click += new System.EventHandler(this.radioVideoSource_CheckedChanged);
            // 
            // btnCam02center
            // 
            this.btnCam02center.Location = new System.Drawing.Point(61, 22);
            this.btnCam02center.Name = "btnCam02center";
            this.btnCam02center.Size = new System.Drawing.Size(31, 23);
            this.btnCam02center.TabIndex = 5;
            this.btnCam02center.Tag = "2c";
            this.btnCam02center.Text = "Cn";
            this.btnCam02center.UseVisualStyleBackColor = true;
            this.btnCam02center.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(278, 28);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(31, 17);
            this.radioButton2.TabIndex = 24;
            this.radioButton2.TabStop = true;
            this.radioButton2.Tag = "2V1";
            this.radioButton2.Text = "1";
            this.radioButton2.UseVisualStyleBackColor = true;
            this.radioButton2.Click += new System.EventHandler(this.radioVideoSource_CheckedChanged);
            // 
            // btnCam02out
            // 
            this.btnCam02out.Location = new System.Drawing.Point(99, 22);
            this.btnCam02out.Name = "btnCam02out";
            this.btnCam02out.Size = new System.Drawing.Size(31, 23);
            this.btnCam02out.TabIndex = 6;
            this.btnCam02out.Tag = "2o";
            this.btnCam02out.Text = "Ou";
            this.btnCam02out.UseVisualStyleBackColor = true;
            this.btnCam02out.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // cam2seconds
            // 
            this.cam2seconds.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cam2seconds.Location = new System.Drawing.Point(136, 22);
            this.cam2seconds.Name = "cam2seconds";
            this.cam2seconds.Size = new System.Drawing.Size(48, 23);
            this.cam2seconds.TabIndex = 7;
            this.cam2seconds.Text = "5";
            // 
            // cam2set
            // 
            this.cam2set.Location = new System.Drawing.Point(196, 22);
            this.cam2set.Name = "cam2set";
            this.cam2set.Size = new System.Drawing.Size(60, 23);
            this.cam2set.TabIndex = 15;
            this.cam2set.Tag = "2";
            this.cam2set.Text = "Config";
            this.cam2set.UseVisualStyleBackColor = true;
            this.cam2set.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btn03Out);
            this.groupBox3.Controls.Add(this.btn03In);
            this.groupBox3.Controls.Add(this.txt3Id);
            this.groupBox3.Controls.Add(this.lblCam03);
            this.groupBox3.Controls.Add(this.btnCam03in);
            this.groupBox3.Controls.Add(this.radioButton3);
            this.groupBox3.Controls.Add(this.btnCam03center);
            this.groupBox3.Controls.Add(this.radioButton4);
            this.groupBox3.Controls.Add(this.btnCam03out);
            this.groupBox3.Controls.Add(this.cam3seconds);
            this.groupBox3.Controls.Add(this.cam3set);
            this.groupBox3.Controls.Add(this.bufCt03);
            this.groupBox3.Location = new System.Drawing.Point(161, 196);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(678, 56);
            this.groupBox3.TabIndex = 28;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Center";
            // 
            // btn03Out
            // 
            this.btn03Out.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn03Out.Location = new System.Drawing.Point(550, 22);
            this.btn03Out.Name = "btn03Out";
            this.btn03Out.Size = new System.Drawing.Size(25, 23);
            this.btn03Out.TabIndex = 37;
            this.btn03Out.Tag = "1";
            this.btn03Out.Text = "64";
            this.btn03Out.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btn03In
            // 
            this.btn03In.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn03In.Location = new System.Drawing.Point(519, 22);
            this.btn03In.Name = "btn03In";
            this.btn03In.Size = new System.Drawing.Size(25, 23);
            this.btn03In.TabIndex = 36;
            this.btn03In.Tag = "1";
            this.btn03In.Text = "32";
            this.btn03In.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txt3Id
            // 
            this.txt3Id.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txt3Id.Location = new System.Drawing.Point(605, 22);
            this.txt3Id.Name = "txt3Id";
            this.txt3Id.Size = new System.Drawing.Size(25, 23);
            this.txt3Id.TabIndex = 29;
            this.txt3Id.Tag = "3";
            this.txt3Id.Text = "3";
            this.txt3Id.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblCam03
            // 
            this.lblCam03.AutoSize = true;
            this.lblCam03.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCam03.Location = new System.Drawing.Point(352, 22);
            this.lblCam03.Name = "lblCam03";
            this.lblCam03.Size = new System.Drawing.Size(47, 20);
            this.lblCam03.TabIndex = 28;
            this.lblCam03.Text = "None";
            // 
            // btnCam03in
            // 
            this.btnCam03in.Location = new System.Drawing.Point(24, 22);
            this.btnCam03in.Name = "btnCam03in";
            this.btnCam03in.Size = new System.Drawing.Size(31, 23);
            this.btnCam03in.TabIndex = 4;
            this.btnCam03in.Tag = "3i";
            this.btnCam03in.Text = "In";
            this.btnCam03in.UseVisualStyleBackColor = true;
            this.btnCam03in.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(315, 28);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(31, 17);
            this.radioButton3.TabIndex = 25;
            this.radioButton3.TabStop = true;
            this.radioButton3.Tag = "3V2";
            this.radioButton3.Text = "2";
            this.radioButton3.UseVisualStyleBackColor = true;
            this.radioButton3.Click += new System.EventHandler(this.radioVideoSource_CheckedChanged);
            // 
            // btnCam03center
            // 
            this.btnCam03center.Location = new System.Drawing.Point(61, 22);
            this.btnCam03center.Name = "btnCam03center";
            this.btnCam03center.Size = new System.Drawing.Size(31, 23);
            this.btnCam03center.TabIndex = 5;
            this.btnCam03center.Tag = "3c";
            this.btnCam03center.Text = "Cn";
            this.btnCam03center.UseVisualStyleBackColor = true;
            this.btnCam03center.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // radioButton4
            // 
            this.radioButton4.AutoSize = true;
            this.radioButton4.Location = new System.Drawing.Point(278, 28);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(31, 17);
            this.radioButton4.TabIndex = 24;
            this.radioButton4.TabStop = true;
            this.radioButton4.Tag = "3V1";
            this.radioButton4.Text = "1";
            this.radioButton4.UseVisualStyleBackColor = true;
            this.radioButton4.Click += new System.EventHandler(this.radioVideoSource_CheckedChanged);
            // 
            // btnCam03out
            // 
            this.btnCam03out.Location = new System.Drawing.Point(99, 22);
            this.btnCam03out.Name = "btnCam03out";
            this.btnCam03out.Size = new System.Drawing.Size(31, 23);
            this.btnCam03out.TabIndex = 6;
            this.btnCam03out.Tag = "3o";
            this.btnCam03out.Text = "Ou";
            this.btnCam03out.UseVisualStyleBackColor = true;
            this.btnCam03out.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // cam3seconds
            // 
            this.cam3seconds.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cam3seconds.Location = new System.Drawing.Point(136, 23);
            this.cam3seconds.Name = "cam3seconds";
            this.cam3seconds.Size = new System.Drawing.Size(48, 23);
            this.cam3seconds.TabIndex = 7;
            this.cam3seconds.Text = "5";
            // 
            // cam3set
            // 
            this.cam3set.Location = new System.Drawing.Point(196, 22);
            this.cam3set.Name = "cam3set";
            this.cam3set.Size = new System.Drawing.Size(60, 23);
            this.cam3set.TabIndex = 15;
            this.cam3set.Tag = "3";
            this.cam3set.Text = "Config";
            this.cam3set.UseVisualStyleBackColor = true;
            this.cam3set.Click += new System.EventHandler(this.btnCam_Click);
            // 
            // bufCt03
            // 
            this.bufCt03.AutoSize = true;
            this.bufCt03.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bufCt03.ForeColor = System.Drawing.Color.Firebrick;
            this.bufCt03.Location = new System.Drawing.Point(637, 15);
            this.bufCt03.Name = "bufCt03";
            this.bufCt03.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.bufCt03.Size = new System.Drawing.Size(26, 29);
            this.bufCt03.TabIndex = 32;
            this.bufCt03.Tag = "3";
            this.bufCt03.Text = "0";
            this.bufCt03.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            this.bufCt03.Click += new System.EventHandler(this.bufCt_Click);
            // 
            // bufCt
            // 
            this.bufCt.AutoSize = true;
            this.bufCt.Font = new System.Drawing.Font("Microsoft Sans Serif", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bufCt.ForeColor = System.Drawing.Color.Firebrick;
            this.bufCt.Location = new System.Drawing.Point(798, 3);
            this.bufCt.Name = "bufCt";
            this.bufCt.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.bufCt.Size = new System.Drawing.Size(51, 55);
            this.bufCt.TabIndex = 29;
            this.bufCt.Text = "0";
            this.bufCt.Click += new System.EventHandler(this.label1_Click);
            // 
            // txtBoxLaps
            // 
            this.txtBoxLaps.Font = new System.Drawing.Font("Microsoft Sans Serif", 32F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtBoxLaps.Location = new System.Drawing.Point(628, 3);
            this.txtBoxLaps.MaxLength = 3;
            this.txtBoxLaps.Name = "txtBoxLaps";
            this.txtBoxLaps.Size = new System.Drawing.Size(96, 56);
            this.txtBoxLaps.TabIndex = 30;
            this.txtBoxLaps.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // txtLogPath
            // 
            this.txtLogPath.Location = new System.Drawing.Point(12, 431);
            this.txtLogPath.Name = "txtLogPath";
            this.txtLogPath.Size = new System.Drawing.Size(405, 20);
            this.txtLogPath.TabIndex = 31;
            this.txtLogPath.Text = "C:\\nginx-rtmp\\www\\";
            // 
            // chkManualOverride
            // 
            this.chkManualOverride.AutoSize = true;
            this.chkManualOverride.Location = new System.Drawing.Point(730, 6);
            this.chkManualOverride.Name = "chkManualOverride";
            this.chkManualOverride.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.chkManualOverride.Size = new System.Drawing.Size(61, 17);
            this.chkManualOverride.TabIndex = 32;
            this.chkManualOverride.Text = "Manual";
            this.chkManualOverride.UseVisualStyleBackColor = true;
            // 
            // chkUp
            // 
            this.chkUp.AutoSize = true;
            this.chkUp.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.chkUp.Location = new System.Drawing.Point(730, 29);
            this.chkUp.Name = "chkUp";
            this.chkUp.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.chkUp.Size = new System.Drawing.Size(40, 17);
            this.chkUp.TabIndex = 33;
            this.chkUp.Text = "Up";
            this.chkUp.UseVisualStyleBackColor = true;
            // 
            // eventTitlesPathFile
            // 
            this.eventTitlesPathFile.Location = new System.Drawing.Point(12, 353);
            this.eventTitlesPathFile.Name = "eventTitlesPathFile";
            this.eventTitlesPathFile.Size = new System.Drawing.Size(405, 20);
            this.eventTitlesPathFile.TabIndex = 34;
            this.eventTitlesPathFile.Text = "C:\\nginx-rtmp\\www\\EventOrderVelodromeCup.txt";
            // 
            // eventTitle
            // 
            this.eventTitle.Location = new System.Drawing.Point(12, 379);
            this.eventTitle.Name = "eventTitle";
            this.eventTitle.Size = new System.Drawing.Size(559, 20);
            this.eventTitle.TabIndex = 35;
            this.eventTitle.Text = "Current Event String";
            // 
            // eventTitleLineItem
            // 
            this.eventTitleLineItem.Location = new System.Drawing.Point(423, 353);
            this.eventTitleLineItem.Name = "eventTitleLineItem";
            this.eventTitleLineItem.Size = new System.Drawing.Size(47, 20);
            this.eventTitleLineItem.TabIndex = 36;
            this.eventTitleLineItem.Text = "1";
            // 
            // eventTitleLinePrev
            // 
            this.eventTitleLinePrev.Location = new System.Drawing.Point(480, 350);
            this.eventTitleLinePrev.Name = "eventTitleLinePrev";
            this.eventTitleLinePrev.Size = new System.Drawing.Size(42, 23);
            this.eventTitleLinePrev.TabIndex = 37;
            this.eventTitleLinePrev.Tag = "";
            this.eventTitleLinePrev.Text = "Prev";
            this.eventTitleLinePrev.UseVisualStyleBackColor = true;
            this.eventTitleLinePrev.Click += new System.EventHandler(this.eventTitleLinePrev_Click);
            // 
            // eventTitleLineNext
            // 
            this.eventTitleLineNext.Location = new System.Drawing.Point(528, 350);
            this.eventTitleLineNext.Name = "eventTitleLineNext";
            this.eventTitleLineNext.Size = new System.Drawing.Size(43, 23);
            this.eventTitleLineNext.TabIndex = 38;
            this.eventTitleLineNext.Tag = "";
            this.eventTitleLineNext.Text = "Next";
            this.eventTitleLineNext.UseVisualStyleBackColor = true;
            this.eventTitleLineNext.Click += new System.EventHandler(this.eventTitleLineNext_Click);
            // 
            // eventTitleLineSave
            // 
            this.eventTitleLineSave.Location = new System.Drawing.Point(577, 379);
            this.eventTitleLineSave.Name = "eventTitleLineSave";
            this.eventTitleLineSave.Size = new System.Drawing.Size(53, 23);
            this.eventTitleLineSave.TabIndex = 39;
            this.eventTitleLineSave.Tag = "";
            this.eventTitleLineSave.Text = "Save";
            this.eventTitleLineSave.UseVisualStyleBackColor = true;
            this.eventTitleLineSave.Click += new System.EventHandler(this.eventTitleLineSave_Click);
            // 
            // eventTitlePathFile
            // 
            this.eventTitlePathFile.Location = new System.Drawing.Point(12, 405);
            this.eventTitlePathFile.Name = "eventTitlePathFile";
            this.eventTitlePathFile.Size = new System.Drawing.Size(405, 20);
            this.eventTitlePathFile.TabIndex = 40;
            this.eventTitlePathFile.Text = "C:\\nginx-rtmp\\www\\CurrentEvent.txt";
            // 
            // ControllerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(924, 453);
            this.Controls.Add(this.eventTitlePathFile);
            this.Controls.Add(this.eventTitleLineSave);
            this.Controls.Add(this.eventTitleLinePrev);
            this.Controls.Add(this.eventTitleLineNext);
            this.Controls.Add(this.eventTitleLineItem);
            this.Controls.Add(this.eventTitle);
            this.Controls.Add(this.eventTitlesPathFile);
            this.Controls.Add(this.chkUp);
            this.Controls.Add(this.chkManualOverride);
            this.Controls.Add(this.txtLogPath);
            this.Controls.Add(this.txtBoxLaps);
            this.Controls.Add(this.bufCt);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textBoxMsg);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.txtComPort);
            this.Controls.Add(this.txtLaps);
            this.Controls.Add(this.LedFrequencyValue);
            this.Controls.Add(this.LedFrequencyLabel);
            this.Controls.Add(this.LedFrequencyLabelTrackBar);
            this.Controls.Add(this.EnableLedCheckBox);
            this.Name = "ControllerForm";
            this.Text = "Arduino Controller";
            this.Load += new System.EventHandler(this.ControllerForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.LedFrequencyLabelTrackBar)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox EnableLedCheckBox;
        private System.Windows.Forms.TrackBar LedFrequencyLabelTrackBar;
        private System.Windows.Forms.Label LedFrequencyLabel;
        private System.Windows.Forms.Label LedFrequencyValue;
        public System.Windows.Forms.Button btnCam01in;
        public System.Windows.Forms.Button btnCam01center;
        public System.Windows.Forms.Button btnCam01out;
        private System.Windows.Forms.TextBox cam1seconds;
        private System.Windows.Forms.Button cam1set;
        private System.Windows.Forms.Label txtLaps;
        private System.Windows.Forms.TextBox txtComPort;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.TextBox textBoxMsg;
        public System.Windows.Forms.RadioButton radioButton1_1;
        public System.Windows.Forms.RadioButton radioButton1_2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        public System.Windows.Forms.Button btnCam02in;
        public System.Windows.Forms.RadioButton radioButton1;
        public System.Windows.Forms.Button btnCam02center;
        public System.Windows.Forms.RadioButton radioButton2;
        public System.Windows.Forms.Button btnCam02out;
        private System.Windows.Forms.TextBox cam2seconds;
        private System.Windows.Forms.Button cam2set;
        private System.Windows.Forms.GroupBox groupBox3;
        public System.Windows.Forms.Button btnCam03in;
        public System.Windows.Forms.RadioButton radioButton3;
        public System.Windows.Forms.Button btnCam03center;
        public System.Windows.Forms.RadioButton radioButton4;
        public System.Windows.Forms.Button btnCam03out;
        private System.Windows.Forms.TextBox cam3seconds;
        private System.Windows.Forms.Button cam3set;
        private System.Windows.Forms.Label lblCam01;
        private System.Windows.Forms.Label lblCam02;
        private System.Windows.Forms.Label lblCam03;
        private System.Windows.Forms.TextBox txt1Id;
        private System.Windows.Forms.TextBox txt2Id;
        private System.Windows.Forms.TextBox txt3Id;
        private System.Windows.Forms.Label bufCt;
        private System.Windows.Forms.TextBox txtBoxLaps;
        private System.Windows.Forms.Label bufCt01;
        private System.Windows.Forms.Label bufCt02;
        private System.Windows.Forms.Label bufCt03;
        public System.Windows.Forms.TextBox txtLogPath;
        private System.Windows.Forms.CheckBox chkManualOverride;
        private System.Windows.Forms.CheckBox chkUp;
        public System.Windows.Forms.TextBox btn01Out;
        public System.Windows.Forms.TextBox btn01In;
        public System.Windows.Forms.TextBox btn02Out;
        public System.Windows.Forms.TextBox btn02In;
        public System.Windows.Forms.TextBox btn03Out;
        public System.Windows.Forms.TextBox btn03In;
        public System.Windows.Forms.TextBox eventTitlesPathFile;
        public System.Windows.Forms.TextBox eventTitle;
        public System.Windows.Forms.TextBox eventTitleLineItem;
        public System.Windows.Forms.Button eventTitleLinePrev;
        public System.Windows.Forms.Button eventTitleLineNext;
        public System.Windows.Forms.Button eventTitleLineSave;
        public System.Windows.Forms.TextBox eventTitlePathFile;
    }
}

