#include <vector>
#include "RNetPacket.hpp"
#include "RNetBuffer.hpp"

int main(int argc, char** argv) {


	rnet::RNetPacket packet1;
	rnet::RNetBuffer buffer;

	std::vector<uint8_t> Message = {0x0b, 0x00, 0x00, 0x10, 0x20};

	
	printf("+ Adding to buffer RNet packet with increasing sequence number...\n");
	for (auto i=0; i<5; i++) {
		packet1.Set(i, rnet::DATAPACKET, Message, Message.size());
		buffer.Add(packet1);
	}


	printf("+ Removing packet with index 3\n");
	buffer.Remove(3);
	// Dumping
	for(auto it = buffer.Begin(); it != buffer.End(); it++) {
		it->DumpRaw();
	}


	printf("+ Removing packet with iterator end()\n");
	auto it = buffer.End();
	buffer.Remove(it);
	
	// Dumping
	for(auto it = buffer.Begin(); it != buffer.End(); it++) {
		it->DumpRaw();
	}




	return 0;

}
