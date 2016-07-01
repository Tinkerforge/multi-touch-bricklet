Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Multi Touch Bricklet

    ' Callback subroutine for touch state callback
    Sub TouchStateCB(ByVal sender As BrickletMultiTouch, ByVal state As Integer)
        Dim s As String = ""

        If ((state And (1 << 12)) = (1 << 12)) Then
            s &= "In proximity, "
        End If

        If ((state And &Hfff) = 0) Then
            s &= "No electrodes touched"
        Else
            s &= "Electrodes "
            For i As Integer = 0 To 11
                If ((state And (1 << i)) = (1 << i)) Then
                    s &= i.ToString() & " "
                End If
            Next
            s &= "touched"
        End If

        Console.WriteLine(s)
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim mt As New BrickletMultiTouch(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register touch state callback to subroutine TouchStateCB
        AddHandler mt.TouchState, AddressOf TouchStateCB

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
