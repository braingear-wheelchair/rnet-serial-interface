#include "RNetPacket.hpp"
#include "RNetBuffer.hpp"
#include "RNetServiceRx.hpp"
#include "RNetServiceXY.hpp"
#include "RNetWriter.hpp"
#include "RNetReader.hpp"
#include "RNetSerial.hpp"
#include "RNetServiceKeyboard.hpp"

#define VELY_FORWARD		20
#define VELY_BACKWARD		40
#define VELX_TURN			60
#define VELY_FORWARD_TURN	20
#define VELX_FORWARD_TURN	30

#define KEY_FORWARD			'w'
#define KEY_BACKWARD		's'
#define KEY_FORWARD_LEFT	'a'
#define KEY_FORWARD_RIGHT	'd'
#define KEY_LEFT			'q'
#define KEY_RIGHT			'e'
#define KEY_STOP			'x'
#define KEY_QUIT			27		// ESC


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

	/*********************************/
	/*   Instatiation rnet objects   */
	/*********************************/
	/* All these classes are required to correctly interface to the chipset */

	/*** RNet serial interface ***/
	rnet::RNetSerial rnet;

	/*** RNet buffers for trasmitting and receiving ***/
	rnet::RNetBuffer tx;
	rnet::RNetBuffer rx;

	/*** RNet threads to read and write ***/
	rnet::RNetReader reader(&rnet, &rx);
	rnet::RNetWriter writer(&rnet, &tx);

	/*** Generic RNet service to acknowledge messages from the chipset ***/
	rnet::RNetServiceRx srvrx(&tx, &rx);

	/*** RNet service to send velocity commands ***/
	rnet::RNetServiceXY srvxy(&tx, &rx);

	/*** Keyboard service to capture key press ***/
	rnet::RNetServiceKeyboard srvkey;


	/*****************************************************/
	/*   Serial port opening and connection negozation   */
	/*****************************************************/
	
	/*** Opening serial port ***/
	if(rnet.Open(port) == false) {
		printf("[%s] Serial port \"%s\" is NOT open.\n", rnet.name().c_str(), port.c_str());
		return EXIT_FAILURE;
	}
	printf("[%s] Serial port %s is open and set with default parameters.\n", rnet.name().c_str(), port.c_str());

	/*** Connecting to the chipset ***/
	printf("[%s] Establishing connection to Rebus Chipset...\n", rnet.name().c_str());	
	if(rnet.Connect() == true) {
		printf("[%s] Connection established.\n", rnet.name().c_str());
	}
	
	/***********************************/
	/*   Starting mandatory services   */
	/***********************************/
	reader.Start();
	writer.Start();
	srvrx.Start();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	/***********************************/
	/*    Starting velocity service    */
	/***********************************/
	srvxy.Start();
	
	/***********************************/
	/*    Starting keyboard service    */
	/***********************************/
	srvkey.Start();

	
	/***********************************/
	/*            Main loop            */
	/***********************************/
	int key;
	int8_t vx = 0;
	int8_t vy = 0;
	bool run = true;
	
	while(run == true) {

		/*** Getting current press key ***/
		srvkey.GetKey(key);

		/*** Select the Vx and Vy velocities accordingly ***/
		switch((char)key) {
			case KEY_FORWARD:
				vx = 0;
				vy = VELY_FORWARD;
				break;
			case KEY_FORWARD_RIGHT:
				vx = VELX_FORWARD_TURN;
				vy = VELY_FORWARD_TURN;
				break;
			case KEY_FORWARD_LEFT:
				vx = -VELX_FORWARD_TURN;
				vy = VELY_FORWARD_TURN;
				break;
			case KEY_RIGHT:
				vx = VELX_TURN;
				vy = 0;
				break;
			case KEY_LEFT:
				vx = -VELX_TURN;
				vy = 0;
				break;
			case KEY_BACKWARD:
				vx = 0;
				vy = -VELY_BACKWARD;
				break;
			case KEY_STOP:
				vx = 0;
				vy = 0;
				break;
			case KEY_QUIT:
				run = false;
				break;
		}
	
		/*** Set the velocities to be sent to the chipset ***/
		if(run == true) {
			srvxy.SetVelocity(vx, vy);
		} else {
			printf("User request to quit\n");
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}




	/*** Stopping the services ***/
	srvkey.Stop();
	srvrx.Stop();
	srvxy.Stop();
	reader.Stop();
	writer.Stop();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	/*** Joining the threads ***/
	srvrx.Join();
	reader.Join();
	writer.Join();
	srvxy.Join();
	srvkey.Join();

	/*** Closing serial port ***/
	printf("[%s] Closing port..\n", rnet.name().c_str());
	rnet.Close();
	printf("[%s] Port closed\n", rnet.name().c_str());

	

	return 0;
}

