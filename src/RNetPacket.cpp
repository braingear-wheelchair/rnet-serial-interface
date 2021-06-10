#ifndef RNETPACKET_CPP
#define RNETPACKET_CPP

#include "RNetPacket.hpp"

namespace rnetserial {

RNetPacket::RNetPacket(void) {
	
	// Initialize Data array
	this->startup_		= 0;
	this->datalength_	= 0;
}

RNetPacket::~RNetPacket(void) {
	//delete[] this->data_;
//	this->delete_data_array();
}

void RNetPacket::Set(uint8_t SeqNum, uint8_t Type, std::vector<uint8_t> Data, uint8_t DataLength, bool Startup) {
	
	// Packet setup
	this->SetStartupFlag(Startup);
	this->SetSeqNum(SeqNum);
	this->SetType(Type);
	this->SetData(Data, DataLength);
}

void RNetPacket::SetStartupFlag(bool startup) {
	this->startup_ = startup;
}

void RNetPacket::SetSeqNum(uint8_t seqnum) {
	this->seqnum_ = seqnum;
}

void RNetPacket::SetType(uint8_t type) {
	this->type_ = type;
}

void RNetPacket::SetData(std::vector<uint8_t> data, uint8_t datalength) {

	// Deleting current data array
	//this->delete_data_array();
	this->data_.clear();
	this->datalength_ = datalength;

	for(auto i=0; i<datalength; i++)
		this->data_.push_back(data.at(i));
}

bool RNetPacket::GetStartupFlag(void) {
	return this->startup_;
}

uint8_t RNetPacket::GetSeqNum(void) {
	return this->seqnum_;
}

uint8_t RNetPacket::GetType(void) {
	return this->type_;
}

uint8_t RNetPacket::GetDataLength(void) {
	return this->datalength_;
}

std::vector<uint8_t> RNetPacket::GetData(void) {
	return this->data_;
}

bool RNetPacket::IsType(uint8_t type) {
	return (type == this->type_);
}

bool RNetPacket::IsSeqNum(uint8_t seqnum) {
	return (seqnum == this->seqnum_);
}

bool RNetPacket::IsStartup(void) {
	return (this->startup_ == 1);
}

/*******************************************************************************
 * Encoding methods
 ******************************************************************************/

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
	Header[1] = Header[1] | this->seqnum_  << 3; 
	Header[1] = Header[1] | this->startup_ << 2;
	Header[1] = Header[1] | this->type_;

	// byte 2: Data Length (in bytes)
	Header[2] = this->datalength_;

	// byte 3: CheckSum CRC 8
	CheckBlock[0] = Header[1];
	CheckBlock[1] = Header[2];
	RNetChecksum::CRC8(&CheckSum, CheckBlock, 2);

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

	uint8_t  Data[this->datalength_ + 2];
	uint16_t CheckSum;

	RNetChecksum::CRC16(&CheckSum, this->data_.data(), this->datalength_);

	for(auto i = 0; i<this->datalength_; i++) {
		Data[i] = this->data_.at(i);
	}

	Data[this->datalength_]   = CheckSum >> 8;
	Data[this->datalength_+1] = CheckSum & 0xFF;


	return std::vector<uint8_t>(Data, Data + this->datalength_ + 2);
}

std::vector<uint8_t> RNetPacket::Encode(void) {

	std::vector<uint8_t> vpacket;
	std::vector<uint8_t> vdata;

	vpacket = this->EncodeHeader();

	if(this->datalength_ > 0)
		vdata = this->EncodeData();

	vpacket.insert(vpacket.end(), vdata.begin(), vdata.end());

	return vpacket;
}


/*******************************************************************************
 * Decoding methods
 ******************************************************************************/

void RNetPacket::DecodeHeader(std::vector<uint8_t> header) {
	this->seqnum_		= (header.at(1) & 0xF8) >> 3;
	this->startup_		= (header.at(1) & 0x4) >> 2;
	this->type_			= (header.at(1) & 0x3);
	this->datalength_	= header.at(2);
}

void RNetPacket::DecodeData(std::vector<uint8_t> data) {

	this->datalength_ = data.size() - 2;
	this->data_.clear();

	for(auto i = 0; i<this->datalength_; i++)
		this->data_.push_back(data.at(i));
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

void RNetPacket::DumpRaw(void) {

	std::vector<uint8_t> vpacket;

	vpacket = this->Encode();

	printf("== RNet Data Packet Raw ==\n");
	printf("[%2d] ", this->seqnum_);
	for(auto i=0; i<vpacket.size(); i++)
		printf("%02x ", vpacket.at(i));
	printf("\n");
	printf("==========================\n");
	printf("\n");
}

void RNetPacket::Dump(void) {

	printf("==== RNet Data Packet ====\n");
	printf("+ Header               \n");
	printf("|- Sequence Number: %2d\n", this->seqnum_);
	printf("|- Startup Flag:    %2d\n", this->startup_);
	printf("|- Descriptor:      %2d\n", this->type_);
	printf("|- Data length:     %2d\n", this->datalength_);
	printf("+ Data                 \n");
	if (this->datalength_ > 0) {
		printf("|- ");
		for(auto i=0; i<this->datalength_; i++) {
				printf("%02x ", this->data_.at(i));
		}
		printf("\n");
	}
	printf("==========================\n");
	printf("\n");
}

}



#endif
