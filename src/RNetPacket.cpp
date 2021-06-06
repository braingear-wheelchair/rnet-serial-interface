#ifndef RNETPACKET_CPP
#define RNETPACKET_CPP

#include "RNetPacket.hpp"

namespace rnetserial {

RNetPacket::RNetPacket(void) {
	
	// Initialize Data array
	this->data_.Data	   = nullptr;
	this->data_.DataLength = 0;
}

RNetPacket::~RNetPacket(void) {
	if(this->data_.Data != nullptr) {
		delete this->data_.Data;
	}
}

void RNetPacket::SetHeader(uint8_t SeqNum, bool StartupFlag, uint8_t Descriptor, uint8_t DataLength) {
	this->header_.SequenceNumber = SeqNum;
	this->header_.StartupFlag    = StartupFlag;
	this->header_.Descriptor     = Descriptor;
	this->header_.DataLength     = DataLength;
}

void RNetPacket::SetData(uint8_t* Data, uint8_t DataLength) {

	this->data_.Data	   = new uint8_t[DataLength];
	this->data_.DataLength = DataLength;

	for(auto i = 0; i<DataLength; i++)
		this->data_.Data[i] = Data[i];
}

PacketHeader* RNetPacket::GetHeader(void) {
	return &(this->header_);
}

PacketData* RNetPacket::GetData(void) {
	return &(this->data_);
}




std::vector<uint8_t> RNetPacket::Encode(void) {

	std::vector<uint8_t> vpacket;
	std::vector<uint8_t> vdata;

	vpacket = this->EncodeHeader();

	if(this->header_.DataLength > 0)
		vdata = this->EncodeData();

	vpacket.insert(vpacket.end(), vdata.begin(), vdata.end());

	return vpacket;
}

void RNetPacket::Decode(std::vector<uint8_t> packet) {

	std::vector<uint8_t> vheader;
	std::vector<uint8_t> vdata;

	vheader.insert(vheader.end(), packet.begin(), packet.begin() + RNETPACKET_SIZE_HEADER);
	vdata.insert(vdata.end(), packet.begin() + RNETPACKET_SIZE_HEADER, packet.end());

	this->DecodeHeader(vheader);

	if (vdata.size() > 0)
		this->DecodeData(vdata);
}

void RNetPacket::DecodeHeader(std::vector<uint8_t> header) {
	this->header_.SequenceNumber = (header.at(1) & 0xF8) >> 3;
	this->header_.StartupFlag	 = (header.at(1) & 0x4) >> 2;
	this->header_.Descriptor	 = (header.at(1) & 0x3);
	this->header_.DataLength	 = header.at(2);
}

void RNetPacket::DecodeData(std::vector<uint8_t> data) {

	this->data_.DataLength = data.size() - 2;
	this->data_.Data	   = new uint8_t[this->data_.DataLength];

	for(auto i = 0; i<this->data_.DataLength; i++)
		this->data_.Data[i] = data.at(i);
}


/*********************************
 * Header Part Format	
 *			bit 7	bit 6	bit 5	bit 4	bit 3	bit 2	bit 1	bit 0	
 * byte 0	SYN	
 * byte 1	SequenceNumber	(5 bits)		StartupFlag (1 bit)	PacketDescriptor (2 bits)	
 * byte 2	DataLength	
 * byte 3	HeaderChecksum	
 ********************************/
std::vector<uint8_t> RNetPacket::EncodeHeader(void) {

	uint8_t Header[RNETPACKET_SIZE_HEADER];
	uint8_t CheckBlock[2];
	uint8_t CheckSum;

	// byte 0: SYN
	Header[0] = RNETPACKET_SYN_DEFAULT;

	// byte 1: SequenceNumber (5 bits), StartupFlag (1 bits), PacketDescriptor (2 bits) 
	Header[1] = Header[1] | this->header_.SequenceNumber << 3; 
	Header[1] = Header[1] | this->header_.StartupFlag << 2;
	Header[1] = Header[1] | this->header_.Descriptor;

	// byte 2: Data Length (in bytes)
	Header[2] = this->header_.DataLength;

	// byte 3: CheckSum CRC 8
	CheckBlock[0] = Header[1];
	CheckBlock[1] = Header[2];
	RNetUtility::CRC8(&CheckSum, CheckBlock, 2);

	Header[3] = CheckSum;

	return std::vector<uint8_t>(Header, Header + RNETPACKET_SIZE_HEADER);
}

/*********************************
 * Data Part Format	
 *						bit 7	bit 6	bit 5	bit 4	bit 3	bit 2	bit 1	bit 0	
 *	bytes 0 to 
 *	(DataLength-1) 
 *	inclusive			Data	
 *	byte DataLength		DataChecksumHigh	
 *	byte (DataLength+1)	DataChecksumLow	
 ********************************/
std::vector<uint8_t> RNetPacket::EncodeData(void) {

	uint8_t  Data[this->data_.DataLength + 2];
	uint16_t CheckSum;

	RNetUtility::CRC16(&CheckSum, this->data_.Data, this->data_.DataLength);

	for(auto i = 0; i<this->data_.DataLength; i++) {
		Data[i] = this->data_.Data[i];
	}

	Data[this->data_.DataLength]   = CheckSum >> 8;
	Data[this->data_.DataLength+1] = CheckSum & 0xFF;


	return std::vector<uint8_t>(Data, Data + this->data_.DataLength + 2);
}

bool RNetPacket::DoesMatch(uint8_t SeqNum) {

	return this->header_.SequenceNumber == SeqNum;

}


void RNetPacket::DumpRaw(void) {

	std::vector<uint8_t> vpacket;

	vpacket = this->Encode();

	printf("[%2d] ", this->header_.SequenceNumber);
	for(auto i=0; i<vpacket.size(); i++)
		printf("%02x ", vpacket.at(i));

	printf("\n");
}

void RNetPacket::Dump(void) {

	printf("==== RNet Data Packet ====\n");
	printf("+ Header               \n");
	printf("|- Sequence Number: %2d\n", this->header_.SequenceNumber);
	printf("|- Descriptor:      %2d\n", this->header_.Descriptor);
	printf("|- Data length:     %2d\n", this->header_.DataLength);
	printf("+ Data                 \n");
	if (this->header_.DataLength > 0) {
		printf("|- ");
		for(auto i=0; i<this->header_.DataLength; i++) {
				printf("%02x ", this->data_.Data[i]);
		}
		printf("\n");
	}
}

}



#endif
