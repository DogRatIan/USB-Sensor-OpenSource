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
        CType(Me.Chart1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.Chart2, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(50, 438)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(27, 12)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Port:"
        '
        'ComboBoxPort
        '
        Me.ComboBoxPort.DisplayMember = "gPortList"
        Me.ComboBoxPort.FormattingEnabled = True
        Me.ComboBoxPort.Location = New System.Drawing.Point(83, 435)
        Me.ComboBoxPort.Name = "ComboBoxPort"
        Me.ComboBoxPort.Size = New System.Drawing.Size(91, 20)
        Me.ComboBoxPort.TabIndex = 0
        Me.ComboBoxPort.ValueMember = "gPortList"
        '
        'ButtonTest
        '
        Me.ButtonTest.Location = New System.Drawing.Point(202, 436)
        Me.ButtonTest.Name = "ButtonTest"
        Me.ButtonTest.Size = New System.Drawing.Size(127, 19)
        Me.ButtonTest.TabIndex = 2
        Me.ButtonTest.Text = "Test"
        Me.ButtonTest.UseVisualStyleBackColor = True
        '
        'ButtonStart
        '
        Me.ButtonStart.Location = New System.Drawing.Point(347, 436)
        Me.ButtonStart.Name = "ButtonStart"
        Me.ButtonStart.Size = New System.Drawing.Size(127, 19)
        Me.ButtonStart.TabIndex = 3
        Me.ButtonStart.Text = "Start"
        Me.ButtonStart.UseVisualStyleBackColor = True
        '
        'ButtonEnd
        '
        Me.ButtonEnd.Enabled = False
        Me.ButtonEnd.Location = New System.Drawing.Point(490, 436)
        Me.ButtonEnd.Name = "ButtonEnd"
        Me.ButtonEnd.Size = New System.Drawing.Size(127, 19)
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
        Me.ListLog.ItemHeight = 12
        Me.ListLog.Location = New System.Drawing.Point(12, 473)
        Me.ListLog.Name = "ListLog"
        Me.ListLog.Size = New System.Drawing.Size(606, 88)
        Me.ListLog.TabIndex = 17
        '
        'Timer1
        '
        Me.Timer1.Interval = 5000
        '
        'BarTemperature
        '
        Me.BarTemperature.ForeColor = System.Drawing.Color.OrangeRed
        Me.BarTemperature.Location = New System.Drawing.Point(180, 379)
        Me.BarTemperature.Name = "BarTemperature"
        Me.BarTemperature.Size = New System.Drawing.Size(438, 22)
        Me.BarTemperature.Step = 1
        Me.BarTemperature.TabIndex = 18
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(10, 382)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(67, 12)
        Me.Label2.TabIndex = 19
        Me.Label2.Text = "Temperature:"
        '
        'Chart1
        '
        Me.Chart1.BackColor = System.Drawing.SystemColors.Control
        ChartArea1.Name = "ChartArea1"
        Me.Chart1.ChartAreas.Add(ChartArea1)
        Me.Chart1.Cursor = System.Windows.Forms.Cursors.Cross
        Me.Chart1.Location = New System.Drawing.Point(9, 12)
        Me.Chart1.Name = "Chart1"
        Me.Chart1.Size = New System.Drawing.Size(608, 178)
        Me.Chart1.TabIndex = 0
        Me.Chart1.TabStop = False
        Me.Chart1.Text = "Chart1"
        '
        'EditTemperature
        '
        Me.EditTemperature.Location = New System.Drawing.Point(83, 379)
        Me.EditTemperature.Name = "EditTemperature"
        Me.EditTemperature.ReadOnly = True
        Me.EditTemperature.Size = New System.Drawing.Size(91, 22)
        Me.EditTemperature.TabIndex = 20
        Me.EditTemperature.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'EditHumidity
        '
        Me.EditHumidity.Location = New System.Drawing.Point(83, 408)
        Me.EditHumidity.Name = "EditHumidity"
        Me.EditHumidity.ReadOnly = True
        Me.EditHumidity.Size = New System.Drawing.Size(91, 22)
        Me.EditHumidity.TabIndex = 22
        Me.EditHumidity.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(25, 411)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(52, 12)
        Me.Label3.TabIndex = 21
        Me.Label3.Text = "Humidity:"
        '
        'BarHumidity
        '
        Me.BarHumidity.ForeColor = System.Drawing.Color.RoyalBlue
        Me.BarHumidity.Location = New System.Drawing.Point(180, 408)
        Me.BarHumidity.Name = "BarHumidity"
        Me.BarHumidity.Size = New System.Drawing.Size(438, 22)
        Me.BarHumidity.Step = 1
        Me.BarHumidity.TabIndex = 23
        '
        'Chart2
        '
        Me.Chart2.BackColor = System.Drawing.SystemColors.Control
        ChartArea2.Name = "ChartArea1"
        Me.Chart2.ChartAreas.Add(ChartArea2)
        Me.Chart2.Cursor = System.Windows.Forms.Cursors.Cross
        Me.Chart2.Location = New System.Drawing.Point(9, 195)
        Me.Chart2.Name = "Chart2"
        Me.Chart2.Size = New System.Drawing.Size(608, 178)
        Me.Chart2.TabIndex = 24
        Me.Chart2.TabStop = False
        Me.Chart2.Text = "Chart2"
        '
        'FormMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(632, 573)
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
        Me.MaximizeBox = False
        Me.MaximumSize = New System.Drawing.Size(640, 600)
        Me.MinimumSize = New System.Drawing.Size(640, 600)
        Me.Name = "FormMain"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "USB-TnH (SHT10) Demo"
        CType(Me.Chart1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.Chart2, System.ComponentModel.ISupportInitialize).EndInit()
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

End Class
