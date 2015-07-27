import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletMultiTouch;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletMultiTouch mt = new BrickletMultiTouch(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current touchState
		int touchState = mt.getTouchState(); // Can throw com.tinkerforge.TimeoutException

		String str = "";

		if((touchState & (1 << 12)) == (1 << 12)) {
			str += "In proximity, ";
		}

		if((touchState & 0xfff) == 0) {
			str += "No electrodes touched" + System.getProperty("line.separator");
		} else {
			str += "Electrodes ";
			for(int i = 0; i < 12; i++) {
				if((touchState & (1 << i)) == (1 << i)) {
					str += i + " ";
				}
			}
			str += "touched" + System.getProperty("line.separator");
		}

		System.out.println(str);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
