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


	rnetserial::RNetPacket packet;
	rnetserial::RNetPacket ack;
	while(1) {
		if(rnet.ReadPacket(packet) == true) {
			printf("Data recv: ");
			packet.DumpRaw();
			ack.SetHeader(packet.GetHeader()->SequenceNumber, 0, rnetserial::PacketType::ACKPACKET, 0);
			rnet.WritePacket(ack);
			printf("Data sent: ");
			ack.DumpRaw();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	rnet.Close();
	printf("[%s] Serial port %s is closed.\n", rnet.name().c_str(), port.c_str());
	return 0;
}
