import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletMultiTouch;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Multi Touch Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletMultiTouch mt = new BrickletMultiTouch(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current touch state
		int state = mt.getTouchState(); // Can throw com.tinkerforge.TimeoutException
		String str = "";

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

		System.out.println(str);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
