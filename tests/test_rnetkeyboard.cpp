#include <vector>
#include <iostream>
#include "RNetPacket.hpp"
#include "RNetBuffer.hpp"
#include "RNetServiceRx.hpp"
#include "RNetServiceXY.hpp"
#include "RNetWriter.hpp"
#include "RNetReader.hpp"
#include "RNetSerial.hpp"
#include "RNetServiceKeyboard.hpp"

#define VEL_FORWARD		20
#define VEL_BACKWARD	40
#define VEL_TURN		60

#define KEY_FORWARD		'w'
#define KEY_BACKWARD	's'
#define KEY_LEFT		'a'
#define KEY_RIGHT		'd'
#define KEY_STOP		'x'
#define KEY_QUIT		'q'

void dump_buffer(rnet::RNetBuffer& buff) {

	buff.Lock();
	for(auto it=buff.Begin(); it != buff.End(); it++)
		it->DumpRaw();
	buff.Unlock();
}



int main(int argc, char** argv) {
	int opt;
	std::string port;
	int input1, input2;

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
	
	rnet::RNetSerial rnet;
	rnet::RNetBuffer tx;
	rnet::RNetBuffer rx;
	rnet::RNetReader SrvSerialRx(&rnet, &rx);
	rnet::RNetWriter SrvSerialTx(&rnet, &tx);
	rnet::RNetServiceRx SrvRx(&tx, &rx);
	rnet::RNetServiceXY SrvXY(&tx, &rx);
	rnet::RNetServiceKeyboard SrvKey;


	if(rnet.Open(port) == false) {
		printf("[%s] Serial port \"%s\" is NOT open.\n", rnet.name().c_str(), port.c_str());
		return EXIT_FAILURE;
	}

	printf("[%s] Serial port %s is open and set with default parameters.\n", rnet.name().c_str(), port.c_str());

	printf("[%s] Establishing connection to Rebus Chipset...\n", rnet.name().c_str());	
	if(rnet.Connect() == true) {
		printf("[%s] Connection established.\n", rnet.name().c_str());
	}
	
	SrvSerialRx.Start();
	SrvSerialTx.Start();
	SrvRx.Start();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	SrvXY.Start();
	SrvKey.Start();

	
	int key;
	int8_t vx = 0;
	int8_t vy = 0;
	bool run = true;
	
	while(run == true) {

		SrvKey.GetKey(key);
		switch((char)key) {
			case KEY_FORWARD:
				vx = 0;
				vy = VEL_FORWARD;
				break;
			case KEY_RIGHT:
				vx = VEL_TURN;
				vy = 0;
				break;
			case KEY_LEFT:
				vx = -VEL_TURN;
				vy = 0;
				break;
			case KEY_BACKWARD:
				vx = 0;
				vy = -VEL_BACKWARD;
				break;
			case KEY_STOP:
				vx = 0;
				vy = 0;
				break;
			case KEY_QUIT:
				run = false;
				break;
		}
		
		if(run == true) {
			//printf("Velocity [vx, vy]: %d, %d\n", vx, vy);
			SrvXY.SetVelocity(vx, vy);
		} else {
			printf("User request to quit\n");
		}



		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}





	printf("Stopping threads...");
	SrvKey.Stop();
	SrvRx.Stop();
	SrvXY.Stop();
	SrvSerialRx.Stop();
	SrvSerialTx.Stop();
	printf("done!\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	printf("SrvRx joining...");
	SrvRx.Join();
	printf("done!\n");
	
	printf("SerialRx joining...");
	SrvSerialRx.Join();
	printf("done!\n");
	
	printf("SerialTx joining...");
	SrvSerialTx.Join();
	printf("done!\n");
	
	printf("SrvXY joining...");
	SrvXY.Join();
	printf("done!\n");

	printf("Key joining...\n");
	SrvKey.Join();
	printf("done!\n");

	//printf("Closing port..\n");
	//rnet.Close();
	//printf("Port closed\n");

	

	return 0;
}
