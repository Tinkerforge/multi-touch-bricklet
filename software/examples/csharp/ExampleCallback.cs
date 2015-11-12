using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback function for touch state callback
	static void TouchStateCB(BrickletMultiTouch sender, int state)
	{
		string str = "";

		if((state & (1 << 12)) == (1 << 12)) {
			str += "In proximity, ";
		}

		if((state & 0xfff) == 0) {
			str += "No electrodes touched";
		} else {
			str += "Electrodes ";
			for(int i = 0; i < 12; i++) {
				if((state & (1 << i)) == (1 << i)) {
					str += i + " ";
				}
			}
			str += "touched";
		}

		Console.WriteLine(str);
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletMultiTouch mt = new BrickletMultiTouch(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register touch state callback to function TouchStateCB
		mt.TouchState += TouchStateCB;

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
