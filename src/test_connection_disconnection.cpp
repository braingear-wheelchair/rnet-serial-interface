#include "RNetSerial.hpp"
#include "RNetServiceRx.hpp"
#include <thread>
#include <chrono>

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

	printf("[%s] Connecting...\n", rnet.name().c_str());
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
	
	std::thread t1(&rnetserial::RNetServiceRx::Run, SrvRx);

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	
	
	
	printf("[%s] Shutting down...\n", rnet.name().c_str());
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	rnet.Shutdown();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	SrvRx->Stop();

	t1.join();
	rnet.Close();
	printf("[%s] Serial port %s is closed.\n", rnet.name().c_str(), port.c_str());
	return 0;
}
