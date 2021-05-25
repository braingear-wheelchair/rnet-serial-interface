#ifndef RNETSERIAL_CPP
#define RNETSERIAL_CPP

#include "RNetSerial.hpp"

namespace rnetserial {

RNetSerial::RNetSerial(const std::string name) {
	this->mutex_.lock();
	this->name_			 = name;
	this->baudrate_		 = LibSerial::BaudRate::BAUD_115200;
	this->charactersize_ = LibSerial::CharacterSize::CHAR_SIZE_8;
	this->flowcontrol_	 = LibSerial::FlowControl::FLOW_CONTROL_NONE;
	this->parity_		 = LibSerial::Parity::PARITY_NONE;
	this->stopbits_		 = LibSerial::StopBits::STOP_BITS_1;

	this->sequence_number_ = 0;
	this->mutex_.unlock();
}

RNetSerial::~RNetSerial(void) {
	this->mutex_.lock();
	if(this->serialport_.IsOpen())
		this->Close();
	this->mutex_.unlock();
}

bool RNetSerial::SetBaudRate(const LibSerial::BaudRate& baudrate) {
	try {
		this->mutex_.lock();
		this->serialport_.SetBaudRate(baudrate);
		this->baudrate_ = baudrate;
		this->mutex_.unlock();
	} catch (std::runtime_error& e) {
		this->mutex_.unlock();
		printf("[%s] Error setting \"Baudrate\": %s.\n", this->name().c_str(), e.what());
		return false;
	}
	return true;
}

bool RNetSerial::SetCharacterSize(const LibSerial::CharacterSize& charactersize) {
	try {
		this->mutex_.lock();
		this->serialport_.SetCharacterSize(charactersize);
		this->charactersize_ = charactersize;
		this->mutex_.unlock();
	} catch (std::runtime_error& e) {
		this->mutex_.unlock();
		printf("[%s] Error setting \"Character Size\": %s.\n", this->name().c_str(), e.what());
		return false;
	}
	return true;
}

bool RNetSerial::SetFlowControl(const LibSerial::FlowControl& flowcontrol) {
	try {
		this->mutex_.lock();
		this->serialport_.SetFlowControl(flowcontrol);
		this->flowcontrol_ = flowcontrol;
		this->mutex_.unlock();
	} catch (std::runtime_error& e) {
		this->mutex_.unlock();
		printf("[%s] Error setting \"Flow Control\": %s.\n", this->name().c_str(), e.what());
		return false;
	}
	return true;
}

bool RNetSerial::SetParity(const LibSerial::Parity& parity) {
	try {
		this->mutex_.lock();
		this->serialport_.SetParity(parity);
		this->parity_ = parity;
		this->mutex_.unlock();
	} catch (std::runtime_error& e) {
		this->mutex_.unlock();
		printf("[%s] Error setting \"Parity\": %s.\n", this->name().c_str(), e.what());
		return false;
	}
	return true;
}

bool RNetSerial::SetStopBits(const LibSerial::StopBits& stopbits) {
	try {
		this->mutex_.lock();
		this->serialport_.SetStopBits(stopbits);
		this->stopbits_ = stopbits;
		this->mutex_.unlock();
	} catch (std::runtime_error& e) {
		this->mutex_.unlock();
		printf("[%s] Error setting \"Stop Bits\": %s.\n", this->name().c_str(), e.what());
		return false;
	}
	return true;
}


bool RNetSerial::SetDefaultParameters(void) {

	bool rbr, rcs, rfc, rp, rsb;


	rbr = this->SetBaudRate(this->baudrate_);
	rcs = this->SetCharacterSize(this->charactersize_);
	rfc = this->SetFlowControl(this->flowcontrol_);
	rp  = this->SetParity(this->parity_);
	rsb = this->SetStopBits(this->stopbits_);
	
	return rbr & rcs & rfc & rp & rsb;
}

bool RNetSerial::Open(const std::string port) {

	bool OpenRes = true;
	bool SetRes  = true;
	try {
		this->mutex_.lock();
		this->serialport_.Open(port);
		this->mutex_.unlock();
	} catch (const LibSerial::OpenFailed& e) {
		this->mutex_.unlock();
		printf("[%s] Error opening the port %s: %s.\n", this->name().c_str(), port.c_str(), e.what());
		OpenRes = false;
	}

	if(OpenRes == true) {
		SetRes = this->SetDefaultParameters();
	}

	if(SetRes == false) {
		printf("[%s] Error setting port \"%s\" with default parameters.\n", this->name().c_str(), port.c_str());
	}

	return OpenRes & SetRes;
}

bool RNetSerial::Connect(void) {

	RNetPacket Startup;
	RNetPacket AckTx;
	RNetPacket AckRx;
	bool rread;

	uint8_t StartupMessageLength = 1;
	uint8_t StartupMessage[StartupMessageLength];

	// Startup packet
	
	StartupMessage[0] = 0x1;
	Startup.SetHeader(this->GetSequence(), 1, PacketType::DATAPACKET, StartupMessageLength);
	Startup.SetData(StartupMessage, StartupMessageLength);

	// Write startup packet
	this->WritePacket(Startup);

	// Wait for ACK packet
	rread = this->ReadPacket(AckRx, ACK_LENGTH, ACK_TIMEOUT);
	
	if(AckRx.DoesMatch(this->GetSequence()) == false) {
		printf("[%s] Error acknowledge packet (SN=%d) does not match with "
			   "the current sequence number (SN=%d).\n", this->name().c_str(), 
				AckRx.GetHeader()->SequenceNumber, this->GetSequence());
		return false;
	}

	// Write ACK packet
	AckTx.SetHeader(this->GetSequence(), 0, PacketType::ACKPACKET, 0);
	this->WritePacket(AckTx);
	
	this->IncrementSequence();

	return true;
}

bool RNetSerial::SendVelocity(int8_t vx, int8_t vy) {

	uint8_t MessageLength = 5;
	uint8_t PacketType = 2;

	uint8_t Message[MessageLength];
	
	Message[0] = 0xb;
	Message[1] = 0x0;
	Message[2] = 0x0;
	Message[3] = (uint8_t)vx;
	Message[4] = (uint8_t)vy;

	// Create RNet data packet
	RNetPacket packet;
	packet.SetHeader(this->GetSequence(), 0, PacketType::DATAPACKET, MessageLength);
	packet.SetData(Message, MessageLength);

	// Write packet on serial port
	this->WritePacket(packet);
	
	this->IncrementSequence();
}

bool RNetSerial::WritePacket(RNetPacket& packet) {

	std::vector<uint8_t> vpacket;

	vpacket = packet.Encode();

	this->mutex_.lock();
	this->serialport_.Write(vpacket);
	this->serialport_.DrainWriteBuffer();
	this->mutex_.unlock();

	packet.DumpRaw();
}

bool RNetSerial::ReadPacket(RNetPacket& packet, size_t NBytes, size_t Timeout) {
	
	std::vector<uint8_t> vheader;
	std::vector<uint8_t> vdata;

	// Header
		try {
			this->mutex_.lock();
			this->serialport_.Read(vheader, 4, 100);
			this->mutex_.unlock();
		} catch (std::runtime_error& e) {
			this->mutex_.unlock();
			return false;
		}

	printf("Header received: ");
	for(auto i=0; i<vheader.size(); i++) 
		printf("%02x ", vheader.at(i));
	printf("\n");


	uint8_t length = RNetUtility::GetDataLength(vheader);
	printf("Length: %d\n", length);

	if (length > 0) {
		try {
			this->mutex_.lock();
			this->serialport_.Read(vdata, length+2, 100);
			this->mutex_.unlock();
		} catch (std::runtime_error& e) {
			this->mutex_.unlock();
		}
		printf("Data received: ");
		for(auto i=0; i<vdata.size(); i++) 
			printf("%02x ", vdata.at(i));
		printf("\n");
	}

	vheader.insert(vheader.end(), vdata.begin(), vdata.end());

	packet.Decode(vheader);


	return true;
}

void RNetSerial::Close(void) {
	this->mutex_.lock();
	this->serialport_.Close();
	this->mutex_.unlock();
}




const std::string RNetSerial::name(void) {
	std::string name;

	this->mutex_.lock();
	name = this->name_;
	this->mutex_.unlock();

	return name;
}

uint8_t RNetSerial::GetSequence(void) {
	uint8_t SeqNum;
	this->mutex_.lock();
	SeqNum = this->sequence_number_;
	this->mutex_.unlock();

	return SeqNum;

}

void RNetSerial::IncrementSequence(void) {

	this->mutex_.lock();
	this->sequence_number_++;
	if(this->sequence_number_ > 31)
		this->sequence_number_ = 0;
	this->mutex_.unlock();
}



}


#endif
