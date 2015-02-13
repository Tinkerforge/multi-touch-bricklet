Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Callback function for touch state
    Sub TouchStateCB(ByVal sender As BrickletMultiTouch, ByVal touchState As Integer)
        Dim s As String = ""

        If ((touchState And (1 << 12)) = (1 << 12)) Then
            s &= "In proximity, "
        End If

        If ((touchState And &Hfff) = 0) Then
            s &= "No electrodes touched" + System.Environment.NewLine
        Else
            s &= "Electrodes "
            For i As Integer = 0 To 11
                If ((touchState And (1 << i)) = (1 << i)) Then
                    s &= i.ToString() & " "
                End If
            Next
            s &= "touched" + System.Environment.NewLine
        End If

        System.Console.WriteLine(s)
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim mt As New BrickletMultiTouch(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register touch state callback to function TouchStateCB
        AddHandler mt.TouchState, AddressOf TouchStateCB

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
