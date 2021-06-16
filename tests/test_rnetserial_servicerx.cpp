#include <vector>
#include <iostream>
#include "RNetPacket.hpp"
#include "RNetBuffer.hpp"
#include "RNetServiceRx.hpp"
#include "RNetReceiver.hpp"
#include "RNetTrasmitter.hpp"
#include "RNetSerial.hpp"

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
	rnet::RNetReceiver SrvSerialRx(&rnet, &rx);
	rnet::RNetTrasmitter SrvSerialTx(&rnet, &tx);
	rnet::RNetServiceRx SrvRx(&tx, &rx);

	SrvRx.Start();
	SrvSerialRx.Start();
	SrvSerialTx.Start();

	if(rnet.Open(port) == false) {
		printf("[%s] Serial port \"%s\" is NOT open.\n", rnet.name().c_str(), port.c_str());
		return EXIT_FAILURE;
	}

	printf("[%s] Serial port %s is open and set with default parameters.\n", rnet.name().c_str(), port.c_str());

	printf("[%s] Establishing connection to Rebus Chipset...\n", rnet.name().c_str());	
	if(rnet.Connect() == true) {
		printf("[%s] Connection established.\n", rnet.name().c_str());
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	SrvRx.Stop();
	SrvSerialRx.Stop();
	SrvSerialTx.Stop();
	rnet.Close();

	return 0;
}
