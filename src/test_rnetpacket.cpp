#include <vector>
#include "RNetPacket.hpp"



int main(int argc, char** argv) {


	
	rnetserial::RNetPacket packet1;
	rnetserial::RNetPacket packet2;

	std::vector<uint8_t> rmessage;

	uint8_t MessageLength = 5;
	uint8_t Message[MessageLength];

	uint8_t vx = 10;
	uint8_t vy = 20;

	Message[0] = 0x0b;
	Message[1] = 0x00;
	Message[2] = 0x00;
	Message[3] = (uint8_t)vx;
	Message[4] = (uint8_t)vy;	

	printf("+ Setting RNet packet with increasing sequence number...\n");
	for (auto i = 0; i<5; i++) {
		packet1.Set(i, rnetserial::DATAPACKET, Message, MessageLength);
	}

	printf("+ Dumping the last message:\n");
	packet1.Dump();

	printf("+ Encoding the last message...\n");
	rmessage = packet1.Encode();

	printf("+ Dumping the encoding result:\n");
	for(auto i=0; i<rmessage.size(); i++)
		printf("%02x ", rmessage.at(i));
	printf("\n");

	printf("+ Decoding the raw message in a new RNet packet...\n");
	packet2.Decode(rmessage);

	printf("+ Dumping the new RNet packet:\n");
	packet2.Dump();


	return 0;
}
