#include <string>
#include <string.h>
#include <errno.h> // Error integer and strerror() function
#include "SerialInterface.hpp"


int main (int argc, char** argv) {

	int opt;
	std::string port;

	while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            port = std::string(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s -p PORT\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

	SerialInterface serial("rnet-serial");

	// Opening serial port
	if(serial.Open(port) < 0) {
		printf("[%s] Error %i from serial port \"%s\": %s\n", serial.name().c_str(), 
															  errno, port.c_str(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("[%s] Serial port \"%s\" is open.\n", serial.name().c_str(), port.c_str());


	// Setup serial port
	switch(serial.Setup()) {
		case -1:
			printf("[%s] Error %i from serial port \"%s\" while getting attributes: %s\n", 
																					serial.name().c_str(), 
																					errno, port.c_str(),
																					strerror(errno));
			break;
		case -2:
			printf("[%s] Error %i from serial port \"%s\" while setting attributes: %s\n", 
																					serial.name().c_str(), 
																					errno, port.c_str(), 
																					strerror(errno));
			break;
		default:
			printf("[%s] Serial port \"%s\" is set.\n", serial.name().c_str(), port.c_str());
			break;
	}


	// Writing on serial port
	ssize_t ret;	
	ret = serial.Write("Hello word\n");
	printf("[%s] Written %ld bytes on serial port \"%s\".\n", serial.name().c_str(), ret, port.c_str());



	// Closing serial port
	serial.Close();
	printf("[%s] Serial port \"%s\" is closed.\n", serial.name().c_str(), port.c_str());


	return 0;



}
