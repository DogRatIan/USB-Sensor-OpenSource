<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FormMain
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim ChartArea1 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim ChartArea2 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim ChartArea3 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ComboBoxPort = New System.Windows.Forms.ComboBox()
        Me.ButtonTest = New System.Windows.Forms.Button()
        Me.ButtonStart = New System.Windows.Forms.Button()
        Me.ButtonEnd = New System.Windows.Forms.Button()
        Me.SerialPort = New System.IO.Ports.SerialPort(Me.components)
        Me.ListLog = New System.Windows.Forms.ListBox()
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.BarTemperature = New System.Windows.Forms.ProgressBar()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Chart1 = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.EditTemperature = New System.Windows.Forms.TextBox()
        Me.EditHumidity = New System.Windows.Forms.TextBox()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.BarHumidity = New System.Windows.Forms.ProgressBar()
        Me.Chart2 = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.EditPressure = New System.Windows.Forms.TextBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.BarPressure = New System.Windows.Forms.ProgressBar()
        Me.Chart3 = New System.Windows.Forms.DataVisualization.Charting.Chart()
        CType(Me.Chart1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.Chart2, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.Chart3, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(67, 601)
        Me.Label1.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(38, 17)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Port:"
        '
        'ComboBoxPort
        '
        Me.ComboBoxPort.DisplayMember = "gPortList"
        Me.ComboBoxPort.FormattingEnabled = True
        Me.ComboBoxPort.Location = New System.Drawing.Point(111, 597)
        Me.ComboBoxPort.Margin = New System.Windows.Forms.Padding(4)
        Me.ComboBoxPort.Name = "ComboBoxPort"
        Me.ComboBoxPort.Size = New System.Drawing.Size(120, 24)
        Me.ComboBoxPort.TabIndex = 0
        Me.ComboBoxPort.ValueMember = "gPortList"
        '
        'ButtonTest
        '
        Me.ButtonTest.Location = New System.Drawing.Point(269, 598)
        Me.ButtonTest.Margin = New System.Windows.Forms.Padding(4)
        Me.ButtonTest.Name = "ButtonTest"
        Me.ButtonTest.Size = New System.Drawing.Size(169, 25)
        Me.ButtonTest.TabIndex = 2
        Me.ButtonTest.Text = "Test"
        Me.ButtonTest.UseVisualStyleBackColor = True
        '
        'ButtonStart
        '
        Me.ButtonStart.Location = New System.Drawing.Point(463, 598)
        Me.ButtonStart.Margin = New System.Windows.Forms.Padding(4)
        Me.ButtonStart.Name = "ButtonStart"
        Me.ButtonStart.Size = New System.Drawing.Size(169, 25)
        Me.ButtonStart.TabIndex = 3
        Me.ButtonStart.Text = "Start"
        Me.ButtonStart.UseVisualStyleBackColor = True
        '
        'ButtonEnd
        '
        Me.ButtonEnd.Enabled = False
        Me.ButtonEnd.Location = New System.Drawing.Point(653, 598)
        Me.ButtonEnd.Margin = New System.Windows.Forms.Padding(4)
        Me.ButtonEnd.Name = "ButtonEnd"
        Me.ButtonEnd.Size = New System.Drawing.Size(169, 25)
        Me.ButtonEnd.TabIndex = 4
        Me.ButtonEnd.Text = "Stop"
        Me.ButtonEnd.UseVisualStyleBackColor = True
        '
        'SerialPort
        '
        Me.SerialPort.BaudRate = 115200
        Me.SerialPort.DtrEnable = True
        Me.SerialPort.ReadTimeout = 500
        Me.SerialPort.WriteTimeout = 500
        '
        'ListLog
        '
        Me.ListLog.FormattingEnabled = True
        Me.ListLog.ItemHeight = 16
        Me.ListLog.Location = New System.Drawing.Point(16, 631)
        Me.ListLog.Margin = New System.Windows.Forms.Padding(4)
        Me.ListLog.Name = "ListLog"
        Me.ListLog.Size = New System.Drawing.Size(807, 116)
        Me.ListLog.TabIndex = 17
        '
        'Timer1
        '
        Me.Timer1.Interval = 5000
        '
        'BarTemperature
        '
        Me.BarTemperature.ForeColor = System.Drawing.Color.OrangeRed
        Me.BarTemperature.Location = New System.Drawing.Point(240, 487)
        Me.BarTemperature.Margin = New System.Windows.Forms.Padding(4)
        Me.BarTemperature.Name = "BarTemperature"
        Me.BarTemperature.Size = New System.Drawing.Size(584, 29)
        Me.BarTemperature.Step = 1
        Me.BarTemperature.TabIndex = 18
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(13, 491)
        Me.Label2.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(94, 17)
        Me.Label2.TabIndex = 19
        Me.Label2.Text = "Temperature:"
        '
        'Chart1
        '
        Me.Chart1.BackColor = System.Drawing.SystemColors.Control
        ChartArea1.Name = "ChartArea1"
        Me.Chart1.ChartAreas.Add(ChartArea1)
        Me.Chart1.Cursor = System.Windows.Forms.Cursors.Cross
        Me.Chart1.Location = New System.Drawing.Point(12, 16)
        Me.Chart1.Margin = New System.Windows.Forms.Padding(4)
        Me.Chart1.Name = "Chart1"
        Me.Chart1.Size = New System.Drawing.Size(811, 150)
        Me.Chart1.TabIndex = 0
        Me.Chart1.TabStop = False
        Me.Chart1.Text = "Chart1"
        '
        'EditTemperature
        '
        Me.EditTemperature.Location = New System.Drawing.Point(112, 491)
        Me.EditTemperature.Margin = New System.Windows.Forms.Padding(4)
        Me.EditTemperature.Name = "EditTemperature"
        Me.EditTemperature.ReadOnly = True
        Me.EditTemperature.Size = New System.Drawing.Size(120, 22)
        Me.EditTemperature.TabIndex = 20
        Me.EditTemperature.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'EditHumidity
        '
        Me.EditHumidity.Location = New System.Drawing.Point(111, 526)
        Me.EditHumidity.Margin = New System.Windows.Forms.Padding(4)
        Me.EditHumidity.Name = "EditHumidity"
        Me.EditHumidity.ReadOnly = True
        Me.EditHumidity.Size = New System.Drawing.Size(120, 22)
        Me.EditHumidity.TabIndex = 22
        Me.EditHumidity.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(41, 529)
        Me.Label3.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(66, 17)
        Me.Label3.TabIndex = 21
        Me.Label3.Text = "Humidity:"
        '
        'BarHumidity
        '
        Me.BarHumidity.ForeColor = System.Drawing.Color.RoyalBlue
        Me.BarHumidity.Location = New System.Drawing.Point(240, 526)
        Me.BarHumidity.Margin = New System.Windows.Forms.Padding(4)
        Me.BarHumidity.Name = "BarHumidity"
        Me.BarHumidity.Size = New System.Drawing.Size(584, 29)
        Me.BarHumidity.Step = 1
        Me.BarHumidity.TabIndex = 23
        '
        'Chart2
        '
        Me.Chart2.BackColor = System.Drawing.SystemColors.Control
        ChartArea2.Name = "ChartArea1"
        Me.Chart2.ChartAreas.Add(ChartArea2)
        Me.Chart2.Cursor = System.Windows.Forms.Cursors.Cross
        Me.Chart2.Location = New System.Drawing.Point(11, 174)
        Me.Chart2.Margin = New System.Windows.Forms.Padding(4)
        Me.Chart2.Name = "Chart2"
        Me.Chart2.Size = New System.Drawing.Size(811, 150)
        Me.Chart2.TabIndex = 24
        Me.Chart2.TabStop = False
        Me.Chart2.Text = "Chart2"
        '
        'EditPressure
        '
        Me.EditPressure.Location = New System.Drawing.Point(112, 559)
        Me.EditPressure.Margin = New System.Windows.Forms.Padding(4)
        Me.EditPressure.Name = "EditPressure"
        Me.EditPressure.ReadOnly = True
        Me.EditPressure.Size = New System.Drawing.Size(120, 22)
        Me.EditPressure.TabIndex = 25
        Me.EditPressure.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(41, 559)
        Me.Label4.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(69, 17)
        Me.Label4.TabIndex = 26
        Me.Label4.Text = "Pressure:"
        '
        'BarPressure
        '
        Me.BarPressure.ForeColor = System.Drawing.Color.RoyalBlue
        Me.BarPressure.Location = New System.Drawing.Point(240, 559)
        Me.BarPressure.Margin = New System.Windows.Forms.Padding(4)
        Me.BarPressure.Maximum = 110000
        Me.BarPressure.Minimum = 30000
        Me.BarPressure.Name = "BarPressure"
        Me.BarPressure.Size = New System.Drawing.Size(584, 29)
        Me.BarPressure.Step = 1
        Me.BarPressure.TabIndex = 27
        Me.BarPressure.Value = 30000
        '
        'Chart3
        '
        Me.Chart3.BackColor = System.Drawing.SystemColors.Control
        ChartArea3.Name = "ChartArea1"
        Me.Chart3.ChartAreas.Add(ChartArea3)
        Me.Chart3.Cursor = System.Windows.Forms.Cursors.Cross
        Me.Chart3.Location = New System.Drawing.Point(13, 329)
        Me.Chart3.Margin = New System.Windows.Forms.Padding(4)
        Me.Chart3.Name = "Chart3"
        Me.Chart3.Size = New System.Drawing.Size(811, 150)
        Me.Chart3.TabIndex = 28
        Me.Chart3.TabStop = False
        Me.Chart3.Text = "Chart3"
        '
        'FormMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(843, 756)
        Me.Controls.Add(Me.Chart3)
        Me.Controls.Add(Me.BarPressure)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.EditPressure)
        Me.Controls.Add(Me.Chart2)
        Me.Controls.Add(Me.BarHumidity)
        Me.Controls.Add(Me.EditHumidity)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.EditTemperature)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.BarTemperature)
        Me.Controls.Add(Me.ListLog)
        Me.Controls.Add(Me.ButtonEnd)
        Me.Controls.Add(Me.ButtonStart)
        Me.Controls.Add(Me.ButtonTest)
        Me.Controls.Add(Me.ComboBoxPort)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.Chart1)
        Me.Margin = New System.Windows.Forms.Padding(4)
        Me.MaximizeBox = False
        Me.MaximumSize = New System.Drawing.Size(851, 789)
        Me.MinimumSize = New System.Drawing.Size(851, 789)
        Me.Name = "FormMain"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "USB-PA (BME280) Demo"
        CType(Me.Chart1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.Chart2, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.Chart3, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents ComboBoxPort As System.Windows.Forms.ComboBox
    Friend WithEvents ButtonTest As System.Windows.Forms.Button
    Friend WithEvents ButtonStart As System.Windows.Forms.Button
    Friend WithEvents ButtonEnd As System.Windows.Forms.Button
    Friend WithEvents SerialPort As System.IO.Ports.SerialPort
    Friend WithEvents ListLog As System.Windows.Forms.ListBox
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents BarTemperature As System.Windows.Forms.ProgressBar
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Chart1 As System.Windows.Forms.DataVisualization.Charting.Chart
    Friend WithEvents EditTemperature As System.Windows.Forms.TextBox
    Friend WithEvents EditHumidity As System.Windows.Forms.TextBox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents BarHumidity As System.Windows.Forms.ProgressBar
    Friend WithEvents Chart2 As System.Windows.Forms.DataVisualization.Charting.Chart
    Friend WithEvents EditPressure As System.Windows.Forms.TextBox
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents BarPressure As System.Windows.Forms.ProgressBar
    Friend WithEvents Chart3 As System.Windows.Forms.DataVisualization.Charting.Chart

End Class
