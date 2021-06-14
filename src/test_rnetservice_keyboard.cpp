#include <vector>
#include <iostream>
#include "RNetServiceKeyboard.hpp"

#define KEY_QUIT		'q'

int main(int argc, char** argv) {
	rnetserial::RNetServiceKeyboard SrvKey;


	SrvKey.Start();

	int key;
	bool run = true;
	
	while(run == true) {

		SrvKey.GetKey(key);
		switch((char)key) {
			case KEY_QUIT:
				run = false;
				break;
			default:
				break;
		}
		
		if(run == false) {
			printf("User request to quit\n");
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	printf("Stopping thread...");
	SrvKey.Stop();
	printf("stopped!\n");
		
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	printf("Key joining...");
	SrvKey.Join();
	printf("done!\n");

	return 0;
}
