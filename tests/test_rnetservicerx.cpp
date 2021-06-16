#include <vector>
#include <iostream>
#include "RNetPacket.hpp"
#include "RNetBuffer.hpp"
#include "RNetServiceRx.hpp"

void dump_buffer(rnet::RNetBuffer& buff) {

	buff.Lock();
	for(auto it=buff.Begin(); it != buff.End(); it++)
		it->DumpRaw();
	buff.Unlock();
}



int main(int argc, char** argv) {


	rnet::RNetPacket packet1;
	rnet::RNetBuffer tx;
	rnet::RNetBuffer rx;
	rnet::RNetServiceRx SrvRx(&tx, &rx);

	std::vector<uint8_t> Message = {0x0b, 0x00, 0x00, 0x10, 0x20};
	int n;
	int iter = 0;


	SrvRx.Start();
	while(true) {

		printf("Current Tx Buffer:\n");
		dump_buffer(tx);

		printf("Current Rx Buffer:\n");
		dump_buffer(rx);


		std::cout << "Press Enter to add a packet to Rx Buffer...\n";
		std::cin.clear();
		std::cin.ignore(1);
		std::cin >> n;

		packet1.Set(iter, rnet::PacketType::DATAPACKET, Message, Message.size());
		rx.Lock();
		rx.Add(packet1);
		rx.Unlock();
	
		iter++;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	SrvRx.Stop();

	return 0;
}
