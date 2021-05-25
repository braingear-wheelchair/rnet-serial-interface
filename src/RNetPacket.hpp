#ifndef RNETPACKET_HPP
#define RNETPACKET_HPP

#include <vector>
#include <string>
#include "RNetDefinitions.hpp"
#include "RNetUtility.hpp"


namespace rnetserial {

struct PacketHeader {
	bool		StartupFlag;
	uint8_t		SequenceNumber;
	uint8_t 	Descriptor;
	uint8_t 	DataLength;
	uint8_t 	CheckSum;
};

struct PacketData {
	uint8_t*	Data;
	uint8_t		DataLength;
	uint16_t	CheckSum;
};

enum PacketType {ACKPACKET, NACKPACKET, DATAPACKET};

class RNetPacket {
	public:
		RNetPacket(void);
		~RNetPacket(void);


		void SetHeader(uint8_t SeqNum, bool StartupFlag, uint8_t Descriptor, uint8_t DataLength);
		void SetData(uint8_t* Message, uint8_t DataLength);

		PacketHeader* GetHeader(void);
		PacketData* GetData(void);

		std::vector<uint8_t> Encode(void);
		void Decode(std::vector<uint8_t> packet);

		bool DoesMatch(uint8_t SeqNum);

		void DumpRaw(void);
		void Dump(void);

	protected:
		std::vector<uint8_t> EncodeHeader(void);
		std::vector<uint8_t> EncodeData(void);
	
		void DecodeHeader(std::vector<uint8_t> packet);
		void DecodeData(std::vector<uint8_t> packet);

	protected:
		PacketHeader	header_;
		PacketData		data_;


};

};


#endif
