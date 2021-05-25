#include "RNetSerial.hpp"
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

	if(rnet.Open(port) == false) {
		printf("[%s] Serial port \"%s\" is NOT open.\n", rnet.name().c_str(), port.c_str());
		return EXIT_FAILURE;
	}

	printf("[%s] Serial port %s is open and set with default parameters.\n", rnet.name().c_str(), port.c_str());
	if(rnet.Connect() == true) {
		printf("[%s] Connection established.\n", rnet.name().c_str());
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//printf("[%s] Press Enter to try to send command...\n", rnet.name().c_str());
	//std::cin >> input2;
	
	int8_t vx = 0;
	int8_t vy = 20;
	int i = 0;

	rnetserial::RNetPacket AckTx;
	rnetserial::RNetPacket PktRx;
	while(i<100) {
		rnet.ReadPacket(PktRx, 7, 0);
		AckTx.SetHeader(PktRx.GetHeader()->SequenceNumber, 0, rnetserial::PacketType::ACKPACKET, 0);
		rnet.WritePacket(AckTx);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		i++;
	}
	
	printf("QUI\n");
	//while(1) {
		rnet.SendVelocity(vx, vy);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	//}


	//for(auto i = 0; i<33; i++)
	//	rnet.SendVelocity(vx, vy);

	rnet.Close();
	printf("[%s] Serial port %s is closed.\n", rnet.name().c_str(), port.c_str());
	return 0;
}
