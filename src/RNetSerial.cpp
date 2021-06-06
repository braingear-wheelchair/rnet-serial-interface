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
	uint8_t SeqNum;

	uint8_t StartupMessageLength = 1;
	uint8_t StartupMessage[StartupMessageLength];

	// Startup packet
	StartupMessage[0] = 0x1;
	this->Lock();
	SeqNum = this->GetSequence();
	this->UnLock();
	Startup.SetHeader(SeqNum, 1, PacketType::DATAPACKET, StartupMessageLength);
	Startup.SetData(StartupMessage, StartupMessageLength);

	// Write startup packet
	this->Lock();
	this->WritePacket(Startup);

	// Wait for ACK packet
	while(this->ReadPacket(AckRx) == false);
	AckRx.Dump();
	
	if(AckRx.DoesMatch(this->GetSequence()) == false) {
		printf("[%s] Error acknowledge packet (SN=%d) does not match with "
			   "the current sequence number (SN=%d).\n", this->name().c_str(), 
				AckRx.GetHeader()->SequenceNumber, this->GetSequence());
		return false;
	}

	// Write ACK packet
	AckTx.SetHeader(this->GetSequence(), 0, PacketType::ACKPACKET, 0);
	this->WritePacket(AckTx);
	this->UnLock();
	
	this->Lock();
	this->IncrementSequence();
	this->UnLock();

	return true;
}

bool RNetSerial::SendVelocity(int8_t vx, int8_t vy) {

	RNetPacket AckRx;
	uint8_t MessageLength = 5;
	uint8_t PacketType = 2;
	uint8_t SeqNum;

	uint8_t Message[MessageLength];

	Message[0] = 0x0b;
	Message[1] = 0x00;
	Message[2] = 0x00;
	Message[3] = (uint8_t)vx;
	Message[4] = (uint8_t)vy;

	printf("Get sequence..\n");
	// Create RNet data packet
	RNetPacket packet;
	this->Lock();
	SeqNum = this->GetSequence();
	this->UnLock();
	packet.SetHeader(SeqNum, 0, PacketType::DATAPACKET, MessageLength);
	packet.SetData(Message, MessageLength);

	printf("Send velocity...\n");
	// Write packet on serial port
	//this->WritePacket(packet);
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	this->Lock();
	//this->WritePacket(packet);
	do {
		this->WritePacket(packet);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		//printf("Data sent: ");
		packet.DumpRaw();
	} while (this->WaitForAck(SeqNum) == false);
	printf("Sent and Ack received\n");
	this->UnLock();


	this->Lock();
	this->IncrementSequence();
	this->UnLock();

}

void RNetSerial::Shutdown(void) {
	RNetPacket AckRx;
	uint8_t MessageLength = 3;
	uint8_t PacketType = 2;
	uint8_t SeqNum;

	uint8_t Message[MessageLength];

	Message[0] = 0x0a;
	Message[1] = 0x04;
	Message[2] = 0x00;

	printf("Get sequence..\n");
	// Create RNet data packet
	RNetPacket packet;
	this->Lock();
	SeqNum = this->GetSequence();
	this->UnLock();
	packet.SetHeader(SeqNum, 0, PacketType::DATAPACKET, MessageLength);
	packet.SetData(Message, MessageLength);

	packet.Dump();
	printf("Send shutdown...\n");

	// Write packet on serial port
	//this->WritePacket(packet);
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	this->Lock();
	//this->WritePacket(packet);
	do {
		this->WritePacket(packet);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//printf("Data sent: ");
		packet.DumpRaw();
	} while (this->WaitForAck(SeqNum) == false);
	printf("Shutodwon sent and Ack received\n");
	this->UnLock();


	this->Lock();
	this->IncrementSequence();
	this->UnLock();



}


bool RNetSerial::WaitForAck(uint8_t SeqNum, int timeout) {

	RNetPacket Ack;
	bool ret = false;
	timer_msecs timer;
	int64_t elapsed;

	timer.tic();
	while(ret == false) {
		if(this->ReadPacket(Ack) == true) {
			if(Ack.GetHeader()->SequenceNumber == SeqNum) {
				//Ack.DumpRaw();
				ret = true;
			}
		}

		if(timer.toc() >= RNETCOMM_ACKTIMEOUT) {
			ret = false;
			printf(">>>>>>>>>>>>>>>>TIMEOUT<<<<<<<<<<<<<<<<<<\n");
			break;
		}

	}

	return ret;
}


bool RNetSerial::WritePacket(RNetPacket& packet) {

	std::vector<uint8_t> vpacket;

	vpacket = packet.Encode();

	this->serialport_.Write(vpacket);
	this->serialport_.DrainWriteBuffer();
}


bool RNetSerial::ReadPacket(RNetPacket& packet) {

	
	std::vector<uint8_t> vpacket;
	std::vector<uint8_t> vdata;
	uint8_t DataLength;

	// Read Header, block until 4 bytes are received or 1 ms is elapsed
	// If no data is received, it returns false
	try {
		this->serialport_.Read(vpacket, 4, 1);
	} catch (std::runtime_error& e) {
		return false;
	}

	// Check header validity (size and checksum)
	if(RNetUtility::IsHeaderValid(vpacket) == false)
		return false;

	// Check Data Part: Data length
	DataLength = RNetUtility::GetDataLength(vpacket);
	
	// If data length is 0, then is an ACK package
	if (DataLength == 0) {
		packet.Decode(vpacket);
		return true;
	}

	// Otherwise read the other byte (DataLength + 2)
	try {
		this->serialport_.Read(vdata, DataLength+2, 1);
	} catch (std::runtime_error& e) {
		return false;
	}

	// Check data part validity
	if(RNetUtility::IsDataValid(vdata) == false)
		return false;

	// A complete packet has arrived
	vpacket.insert(vpacket.end(), vdata.begin(), vdata.end());
	packet.Decode(vpacket);

	return true;

}

bool RNetSerial::ReadPacket(RNetPacket& packet, size_t NBytes, size_t Timeout) {
	
	std::vector<uint8_t> vheader;
	std::vector<uint8_t> vdata;

	// Header
		try {
			this->serialport_.Read(vheader, 4, Timeout);
		} catch (std::runtime_error& e) {
			//printf("exit from reading header, mutex unlocked\n");
			return false;
		}

	//printf("Header received: ");
	//for(auto i=0; i<vheader.size(); i++) 
	//	printf("%02x ", vheader.at(i));
	//printf("\n");


	uint8_t length = RNetUtility::GetDataLength(vheader);
	//printf("Length: %d\n", length);

	if (length > 0) {
		try {
			std::lock_guard<std::mutex> lg(this->mutex_);
			this->serialport_.Read(vdata, length+2, Timeout);
		} catch (std::runtime_error& e) {
			return false;
		}
		//printf("Data received: ");
		//for(auto i=0; i<vdata.size(); i++) 
		//	printf("%02x ", vdata.at(i));
		//printf("\n");
	} else {
		return false;
	}

	vheader.insert(vheader.end(), vdata.begin(), vdata.end());
	printf("Data received: ");
	for(auto i=0; i<vheader.size(); i++) 
		printf("%02x ", vheader.at(i));
	printf("\n");

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

	name = this->name_;

	return name;
}

uint8_t RNetSerial::GetSequence(void) {
	uint8_t SeqNum;
	
	SeqNum = this->sequence_number_;

	return SeqNum;

}

void RNetSerial::SetSequence(uint8_t SeqNum) {

	this->sequence_number_ = SeqNum;

}

void RNetSerial::IncrementSequence(void) {

	this->sequence_number_++;
	if(this->sequence_number_ > 31)
		this->sequence_number_ = 0;
}

void RNetSerial::Lock(void) {
	this->mutex_.lock();
}

void RNetSerial::UnLock(void) {
	this->mutex_.unlock();
}



}


#endif
