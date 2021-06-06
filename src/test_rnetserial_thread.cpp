#include "RNetSerial.hpp"
#include "RNetServiceRx.hpp"
#include "RNetServiceKeyboard.hpp"
#include <thread>
#include <chrono>

#define VEL_FORWARD		20
#define VEL_BACKWARD	40
#define VEL_TURN		60

#define KEY_FORWARD		'w'
#define KEY_BACKWARD	's'
#define KEY_LEFT		'a'
#define KEY_RIGHT		'd'
#define KEY_STOP		'x'
#define KEY_QUIT		'q'

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


	rnetserial::RNetSerial rnet;

	if(rnet.Open(port) == false) {
		printf("[%s] Serial port \"%s\" is NOT open.\n", rnet.name().c_str(), port.c_str());
		return EXIT_FAILURE;
	}

	printf("[%s] Serial port %s is open and set with default parameters.\n", rnet.name().c_str(), port.c_str());
	if(rnet.Connect() == true) {
		printf("[%s] Connection established.\n", rnet.name().c_str());
	} else {
		return EXIT_FAILURE;
	}

	rnetserial::RNetServiceRx* SrvRx = new rnetserial::RNetServiceRx(&rnet);
	rnetserial::RNetServiceKeyboard* SrvKey = new rnetserial::RNetServiceKeyboard();
	
	std::thread t1(&rnetserial::RNetServiceRx::Run, SrvRx);
	std::thread t2(&rnetserial::RNetServiceKeyboard::Run, SrvKey);

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	
	int key;
	int8_t vx = 0;
	int8_t vy = 0;
	bool run = true;

	printf("QUI\n");
	while(run == true) {

		SrvKey->GetKey(key);
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
			printf("Velocity [vx, vy]: %d, %d\n", vx, vy);
			rnet.SendVelocity(vx, vy);
		} else {
			printf("User request to quit\n");
		}




		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	
	SrvRx->Stop();
	SrvKey->Stop();


	//for(auto i = 0; i<33; i++)
	//	rnet.SendVelocity(vx, vy);

	t1.join();
	t2.join();
	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	rnet.Shutdown();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	rnet.Close();
	printf("[%s] Serial port %s is closed.\n", rnet.name().c_str(), port.c_str());
	return 0;
}
