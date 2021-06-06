#include "RNetServiceKeyboard.hpp"
#include <thread>
#include <chrono>

#define VEL_FORWARD		20
#define VEL_BACKWARD	30
#define VEL_TURN		60

#define KEY_FORWARD		'w'
#define KEY_BACKWARD	's'
#define KEY_LEFT		'a'
#define KEY_RIGHT		'd'
#define KEY_STOP		'x'
#define KEY_QUIT		'q'

int main(int argc, char** argv) {

	rnetserial::RNetServiceKeyboard* SrvKey = new rnetserial::RNetServiceKeyboard();
	
	std::thread t1(&rnetserial::RNetServiceKeyboard::Run, SrvKey);

	int key;
	int vx, vy;
	bool run = true;
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
		} else {
			SrvKey->Stop();
			printf("User request to quit\n");
		}


		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
			
	t1.join();


	return 0;
}

