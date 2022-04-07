#ifndef RNETSERIAL_CPP
#define RNETSERIAL_CPP

#include "RNetSerial.hpp"

namespace rnet {

RNetSerial::RNetSerial(const std::string name) {

	this->mutex_.lock();

	this->name_			 = name;
	//this->sequence_number_ = 0;

	this->mutex_.unlock();
}

RNetSerial::~RNetSerial(void) {
	this->mutex_.lock();
	if(LibSerial::SerialPort::IsOpen())
		this->Close();
	this->mutex_.unlock();
}



bool RNetSerial::Open(const std::string port) {

	bool OpenRes = true;
	bool SetRes  = true;
	
	// Serial Port opening
	try {
		this->mutex_.lock();
		LibSerial::SerialPort::Open(port);
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

bool RNetSerial::IsOpen(void) {
	bool ret = false;

	this->mutex_.lock();
	ret = LibSerial::SerialPort::IsOpen();
	this->mutex_.unlock();

	return ret;
}

void RNetSerial::Close(void) {
	this->mutex_.lock();
	LibSerial::SerialPort::Close();
	this->mutex_.unlock();
}


bool RNetSerial::Connect(int timeout) {

	RNetPacket Startup;
	RNetPacket AckTx;
	RNetPacket AckRx;

	std::vector<uint8_t> StartupMessage = {RNETPACKET_STARTUP_MESSAGE};
	std::vector<uint8_t> ackdata;

	// Start startup sequence: Write Startup -> Wait for Ack -> Check Ack -> Write Ack
	this->Lock();
		// Create startup packet
		Startup.Set(RNetCounter::Instance().Get(), PacketType::DATAPACKET, StartupMessage, StartupMessage.size(), 1);
		// Write startup packet
		this->WritePacket(Startup);
		//Startup.Dump();
		
		// Wait for ACK packet
		while(this->ReadPacket(AckRx) == false);
		
		// Check Ack
		if(AckRx.GetSeqNum() != RNetCounter::Instance().Get()) {
			printf("[%s] Error acknowledge packet (SN=%d) does not match with "
				   "the current sequence number (SN=%d).\n", this->name().c_str(), 
					AckRx.GetSeqNum(), RNetCounter::Instance().Get());
			return false;
		}

		// Write ACK packet
		AckTx.Set(RNetCounter::Instance().Get(), PacketType::ACKPACKET, ackdata, 0);
		this->WritePacket(AckTx);
	
		// Increment sequence
		RNetCounter::Instance().Increment();
	this->Unlock();

	return true;
}


bool RNetSerial::WritePacket(RNetPacket& packet) {

	std::vector<uint8_t> vpacket;

	vpacket = packet.Encode();

	this->Write(vpacket);
	this->DrainWriteBuffer();

	return true;
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

const std::string RNetSerial::name(void) {
	std::string name;

	name = this->name_;

	return name;
}

void RNetSerial::Lock(void) {
	this->mutex_.lock();
}

void RNetSerial::Unlock(void) {
	this->mutex_.unlock();
}



}


#endif
