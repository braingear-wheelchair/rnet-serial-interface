#ifndef RNET_SERIAL_CPP
#define RNET_SERIAL_CPP

#include "RNetSerial.hpp"

namespace rnetserial {

RNetSerial::RNetSerial(const std::string name) {

	this->mutex_.lock();

	this->name_			 = name;
	this->sequence_number_ = 0;

	this->mutex_.unlock();
}

RNetSerial::~RNetSerial(void) {
	this->mutex_.lock();
	if(this->IsOpen())
		this->Close();
	this->mutex_.unlock();
}



bool RNetSerial::OpenPort(const std::string port) {

	bool OpenRes = true;
	bool SetRes  = true;
	
	// Serial Port opening
	try {
		this->mutex_.lock();
		this->Open(port);
		this->mutex_.unlock();
	} catch (LibSerial::OpenFailed& e) {
		this->mutex_.unlock();
		printf("[%s] Error opening the port %s: %s.\n", this->name().c_str(), port.c_str(), e.what());
		OpenRes = false;
	}
	
	// Serial Port initialization
	try {
		this->mutex_.lock();
		this->SetBaudRate(LibSerial::BaudRate::BAUD_115200);
		this->SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
		this->SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
		this->SetParity(LibSerial::Parity::PARITY_NONE);
		this->SetStopBits(LibSerial::StopBits::STOP_BITS_1);
		this->mutex_.unlock();
	} catch (LibSerial::NotOpen& e) {
		this->mutex_.unlock();
		printf("[%s] Error setting the port %s: %s\n", this->name().c_str(), port.c_str(), e.what());
		SetRes = false;
	}

	return OpenRes & SetRes;
}

void RNetSerial::ClosePort(void) {
	this->mutex_.lock();
	this->Close();
	this->mutex_.unlock();
}


bool RNetSerial::Connect(int timeout) {

	RNetPacket Startup;
	RNetPacket AckTx;
	RNetPacket AckRx;

	uint8_t StartupMessage[RNETPACKET_STARTUP_SIZE] = {RNETPACKET_STARTUP_MESSAGE};

	// Start startup sequence: Write Startup -> Wait for Ack -> Check Ack -> Write Ack
	this->Lock();
		// Create startup packet
		Startup.SetHeader(this->GetSequence(), RNETPACKET_STARTUP_FLAG, 
						  PacketType::DATAPACKET, RNETPACKET_STARTUP_SIZE);
		Startup.SetData(StartupMessage, RNETPACKET_STARTUP_SIZE);

		// Write startup packet
		this->WritePacket(Startup);
		Startup.Dump();
		// Wait for ACK packet
		while(this->ReadPacket(AckRx) == false);
		//if(this->WaitForAck(this->GetSequence(), timeout) == false) {
		//	printf("[%s] Error acknowledge packet (SN=%d) does not match with "
		//		   "the current sequence number (SN=%d).\n", this->name().c_str(), 
		//			AckRx.GetHeader()->SequenceNumber, this->GetSequence());
		//	this->UnLock();
		//	return false;
		//}
		
		// Check Ack
		if(AckRx.DoesMatch(this->GetSequence()) == false) {
			printf("[%s] Error acknowledge packet (SN=%d) does not match with "
				   "the current sequence number (SN=%d).\n", this->name().c_str(), 
					AckRx.GetHeader()->SequenceNumber, this->GetSequence());
			return false;
		}

		// Write ACK packet
		AckTx.SetHeader(this->GetSequence(), 0, PacketType::ACKPACKET, 0);
		this->WritePacket(AckTx);
	
		// Increment sequence
		this->IncrementSequence();
	this->UnLock();

	return true;
}


bool RNetSerial::WritePacket(RNetPacket& packet) {

	std::vector<uint8_t> vpacket;

	vpacket = packet.Encode();

	this->Write(vpacket);
	this->DrainWriteBuffer();
}

bool RNetSerial::ReadPacket(RNetPacket& packet) {

	
	std::vector<uint8_t> vpacket;
	std::vector<uint8_t> vdata;
	uint8_t DataLength;

	// Read Header, block until 4 bytes are received or 1 ms is elapsed
	// If no data is received, it returns false
	try {
		this->Read(vpacket, 4, 1);
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
		this->Read(vdata, DataLength+2, 1);
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

bool RNetSerial::WaitForAck(uint8_t SeqNum, int timeout) {

	RNetPacket Ack;
	bool ret = false;
	timer_msecs timer;
	int64_t elapsed;

	timer.tic();
	while(ret == false) {
		if(this->ReadPacket(Ack) == true) {
			if(Ack.GetHeader()->SequenceNumber == SeqNum) {
				Ack.Dump();
				ret = true;
				break;
			}
		}

		//if(timeout > 0) {
			if(timer.toc() >= timeout) {
				ret = false;
				printf(">>>>>>>>>>>>>>>>TIMEOUT<<<<<<<<<<<<<<<<<<\n");
				break;
			}
		//}

	}

	return ret;
}


/*
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
*/





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
