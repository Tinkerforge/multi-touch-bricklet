#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_multi_touch.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	MultiTouch mt;
	multi_touch_create(&mt, UID, &ipcon); 

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	// Get current touch_state
	uint16_t touch_state;
	if(multi_touch_get_touch_state(&mt, &touch_state) < 0) {
		fprintf(stderr, "Could not get value, probably timeout\n");
		exit(1);
	}

	if(touch_state & (1 << 12)) {
		printf("In proximity, ");
	}

	if((touch_state & 0xfff) == 0) {
		printf("No electrodes touched\n\n");
	} else {
		printf("Electrodes ");
		int i = 0;
		for(; i < 12; i++) {
			if(touch_state & (1 << i)) {
				printf("%d ", i);
			}
		}
		printf("touched\n\n");
	}

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
