#include <vector>
#include "RNetPacket.hpp"



int main(int argc, char** argv) {


	
	rnetserial::RNetPacket packet1;
	rnetserial::RNetPacket packet2;

	std::vector<uint8_t> rmessage;


	std::vector<uint8_t> Message = {0x0b, 0x00, 0x00, 0x10, 0x20};

	printf("+ Setting RNet packet with increasing sequence number...\n");
	for (auto i = 0; i<5; i++) {
		packet1.Set(i, rnetserial::DATAPACKET, Message, Message.size());
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
