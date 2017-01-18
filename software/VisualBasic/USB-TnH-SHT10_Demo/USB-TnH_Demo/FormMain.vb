Imports System
Imports System.IO.Ports
Imports System.Threading
Imports System.Windows.Forms.DataVisualization.Charting

Public Class FormMain

    Dim gWithHumi As Boolean = False
    Dim gCurrHumi As Double = 0
    Dim gCurrTemp As Double = 0
    Dim gChartTemp As Array = Array.CreateInstance(GetType(Double), 60)
    Dim gChartHumi As Array = Array.CreateInstance(GetType(Double), 60)
    Dim gPortList As String() = SerialPort.GetPortNames

    Private Sub Log(ByVal str As String)
        Me.ListLog.Items.Insert(0, str)
        REM Limit number of lines on log
        If Me.ListLog.Items.Count > 100 Then
            Me.ListLog.Items.RemoveAt(Me.ListLog.Items.Count - 1)
        End If
    End Sub

    Private Function CleanUpString(ByVal str As String) As String
        str = str.Replace(vbLf, " ")
        str = str.Replace(vbCr, " ")
        str = str.Trim()
        Return str
    End Function

    Private Sub UpdateEditBox()
        Dim str_temp As String
        Dim str_humi As String

        REM Update Temperature
        If gCurrTemp = Double.NaN Then
            Me.BarTemperature.Value = 0
            str_temp = ""
        Else
            If gCurrTemp < 0 Then
                Me.BarTemperature.ForeColor = Color.Aqua
                Me.BarTemperature.Value = 0
            ElseIf gCurrTemp > Me.BarTemperature.Maximum Then
                Me.BarTemperature.ForeColor = Color.Red
                Me.BarTemperature.Value = Me.BarTemperature.Maximum
            Else
                Me.BarTemperature.ForeColor = Color.RoyalBlue
                Me.BarTemperature.Value = gCurrTemp
            End If

            str_temp = String.Format("{0:F1} C", gCurrTemp)
        End If
        Me.BarTemperature.Refresh()
        Me.EditTemperature.Text = str_temp

        REM Update Humidity
        If Me.gWithHumi = False Then
            Me.BarHumidity.Value = 0
            str_humi = "nil"
        Else
            If gCurrHumi = Double.NaN Then
                Me.BarHumidity.Value = 0
                str_humi = ""
            Else
                If gCurrHumi < 0 Then
                    Me.BarHumidity.ForeColor = Color.Aqua
                    Me.BarHumidity.Value = 0
                ElseIf gCurrHumi > Me.BarHumidity.Maximum Then
                    Me.BarHumidity.ForeColor = Color.Red
                    Me.BarHumidity.Value = Me.BarHumidity.Maximum
                Else
                    Me.BarHumidity.ForeColor = Color.RoyalBlue
                    Me.BarHumidity.Value = gCurrHumi
                End If

                str_humi = String.Format("{0:F1} %RH", gCurrHumi)
            End If
        End If
        Me.BarHumidity.Refresh()
        Me.EditHumidity.Text = str_humi

    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim i As Integer

        REM Setup ComboBox of port
        Me.ComboBoxPort.Items.AddRange(gPortList)

        REM Init chart data
        For i = 1 To 59
            gChartTemp(i) = Double.NaN
            gChartHumi(i) = Double.NaN
        Next i
        Me.Chart1.DataBindTable(gChartTemp)
        Me.Chart1.Series(0).ChartType = 4
        Me.Chart2.DataBindTable(gChartHumi)
        Me.Chart2.Series(0).ChartType = 4

    End Sub

    Private Function GetTemperature(ByVal portName As String) As Double
        Dim str_temp As String
        Dim ret As Double = Double.NaN

        ret = 0
        Try
            REM Check setting
            If portName = vbNullString Then
                Log("Please select port.")
                Return Double.NaN
            End If
            If portName.Length = 0 Then
                Log("Please select port.")
                Return Double.NaN
            End If

            REM Config Serial Port
            If (Me.SerialPort.IsOpen = False) Then
                Thread.Sleep(100)

                Me.SerialPort.PortName = portName
                Me.SerialPort.Open()
                Me.SerialPort.ReadTimeout = 500
            End If

            REM Get Temperature
            Me.SerialPort.ReadExisting()
            Me.SerialPort.Write("GT" + vbCr)
            str_temp = CleanUpString(Me.SerialPort.ReadLine)
            ret = str_temp


        Catch
            Log(Err.Description)
        End Try

        Return ret
    End Function

    Private Function GetHumidity(ByVal portName As String) As Double
        Dim str_humi As String
        Dim ret As Double = Double.NaN

        ret = 0
        Try
            REM Check setting
            If portName = vbNullString Then
                Log("Please select port.")
                Return Double.NaN
            End If
            If portName.Length = 0 Then
                Log("Please select port.")
                Return Double.NaN
            End If

            REM Config Serial Port
            If (Me.SerialPort.IsOpen = False) Then
                Thread.Sleep(100)

                Me.SerialPort.PortName = portName
                Me.SerialPort.Open()
                Me.SerialPort.ReadTimeout = 500
            End If

            REM Get Temperature
            Me.SerialPort.ReadExisting()
            Me.SerialPort.Write("GH" + vbCr)
            str_humi = CleanUpString(Me.SerialPort.ReadLine)
            ret = str_humi


        Catch
            Log(Err.Description)
        End Try

        Return ret
    End Function

    Private Function TestPort(ByVal portName As String, Optional ByVal showError As Boolean = True) As String
        Dim str_info As String
        Dim str_version As String
        Dim str_temp As String
        Dim str_humi As String
        Dim ret As Boolean
        Dim ch As Char

        REM init var
        ret = False
        str_info = "Unknown"
        str_version = ""
        Try
            REM Check setting
            If portName = vbNullString Then
                Log("Please select port.")
                Return vbNullString
            End If
            If portName.Length = 0 Then
                Log("Please select port.")
                Return vbNullString
            End If

            REM Config Serial Port
            If (Me.SerialPort.IsOpen) Then
                Me.SerialPort.Close()
                Thread.Sleep(100)
            End If

            Me.SerialPort.PortName = portName
            Me.SerialPort.Open()
            Me.SerialPort.ReadTimeout = 500

            REM Get info
            Me.SerialPort.ReadExisting()
            Me.SerialPort.Write("GI" + vbCr)
            str_info = CleanUpString(Me.SerialPort.ReadLine)
            If (str_info = "USB-TnH LM75") Then
                REM Get version
                Me.SerialPort.ReadExisting()
                Me.SerialPort.Write("GV" + vbCr)
                str_version = CleanUpString(Me.SerialPort.ReadLine)
                ch = str_version.Chars(0)
                If (ch = "V") Then
                    ret = True
                End If
                Me.gWithHumi = False
            End If
            If (str_info = "USB-TnH SHT10") Then
                REM Get version
                Me.SerialPort.ReadExisting()
                Me.SerialPort.Write("GV" + vbCr)
                str_version = CleanUpString(Me.SerialPort.ReadLine)
                ch = str_version.Chars(0)
                If (ch = "V") Then
                    ret = True
                End If
                Me.gWithHumi = True
            End If

            REM Get Temperature
            If (ret = True) Then
                Me.SerialPort.ReadExisting()
                Me.SerialPort.Write("GT" + vbCr)
                str_temp = CleanUpString(Me.SerialPort.ReadLine)
                gCurrTemp = str_temp
            End If

            REM Get Humidity
            If (ret = True) And (gWithHumi) Then
                Me.SerialPort.ReadExisting()
                Me.SerialPort.Write("GH" + vbCr)
                str_humi = CleanUpString(Me.SerialPort.ReadLine)
                gCurrHumi = str_humi
            End If


            Me.SerialPort.Close()
        Catch
            If (showError) Then
                Log(Err.Description)
            End If
        End Try

        If (ret = True) Then
            Return (str_info & " " & str_version)
        Else
            Return ""
        End If
    End Function

    Private Sub ButtonTest_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonTest.Click
        Dim str As String

        REM Test Port
        str = TestPort(Me.ComboBoxPort.Text)
        If (str = vbNullString) Then
            Exit Sub
        End If

        If (str.Length = 0) Then
            Log("No device found at " & Me.ComboBoxPort.Text)
        Else
            Log("'" & str & "' found at " & Me.ComboBoxPort.Text)
            UpdateEditBox()
        End If
    End Sub

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        Dim i As Integer
        Dim len As Integer

        gCurrTemp = GetTemperature(Me.ComboBoxPort.Text)
        gCurrHumi = GetHumidity(Me.ComboBoxPort.Text)

        REM Update chart
        len = gChartTemp.Length
        For i = 0 To (len - 2)
            gChartTemp(i) = gChartTemp(i + 1)
            gChartHumi(i) = gChartHumi(i + 1)
        Next i
        gChartTemp(len - 1) = gCurrTemp
        gChartHumi(len - 1) = gCurrHumi

        Me.Chart1.Series.Clear()
        Me.Chart1.DataBindTable(gChartTemp)
        Me.Chart1.ResetAutoValues()
        Me.Chart1.Series(0).ChartType = 4

        Me.Chart2.Series.Clear()
        Me.Chart2.DataBindTable(gChartHumi)
        Me.Chart2.ResetAutoValues()
        Me.Chart2.Series(0).ChartType = 4

        REM Update edit box
        UpdateEditBox()

    End Sub

    Private Sub ButtonStart_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonStart.Click
        Dim str As String
        Dim i As Integer

        REM Clear chart
        For i = 0 To 59
            gChartTemp(i) = Double.NaN
            gChartHumi(i) = Double.NaN
        Next i
        Me.Chart1.Series.Clear()
        Me.Chart1.DataBindTable(gChartTemp)
        Me.Chart1.Series(0).ChartType = 4
        Me.Chart2.Series.Clear()
        Me.Chart2.DataBindTable(gChartHumi)
        Me.Chart2.Series(0).ChartType = 4

        REM Test Port
        str = TestPort(Me.ComboBoxPort.Text)
        If (str = vbNullString) Then
            Exit Sub
        End If
        If (str.Length = 0) Then
            Log("No device found at " & Me.ComboBoxPort.Text)
            Exit Sub
        End If
        Log("'" & str & "' found at " & Me.ComboBoxPort.Text)
        UpdateEditBox()

        REM Start timer
        Me.Timer1.Interval = 5000
        Me.Timer1.Start()
        Me.ComboBoxPort.Enabled = False
        Me.ButtonStart.Enabled = False
        Me.ButtonTest.Enabled = False
        Me.ButtonEnd.Enabled = True

        Log("Timer started")
    End Sub

    Private Sub ButtonEnd_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonEnd.Click
        Me.Timer1.Stop()
        Me.ComboBoxPort.Enabled = True
        Me.ButtonStart.Enabled = True
        Me.ButtonTest.Enabled = True
        Me.ButtonEnd.Enabled = False

        REM Close port
        If Me.SerialPort.IsOpen = True Then
            Me.SerialPort.Close()
        End If

        Log("Timer stopped")
    End Sub

End Class
