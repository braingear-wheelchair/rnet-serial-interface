#ifndef RNETSERVICE_XY_CPP
#define RNETSERVICE_XY_CPP

#include "RNetServiceXY.hpp"

namespace rnetserial {

RNetServiceXY::RNetServiceXY(RNetSerial* serial) {
	this->serial_ = serial;
	this->vx_ = 0;
	this->vy_ = 0;
}

RNetServiceXY::~RNetServiceXY(void) {
}

void RNetServiceXY::Run(void) {

	RNetPacket AckTx;
	RNetPacket PktRx;
	int8_t vx, vy;
	
	while(this->IsRunning()) {

		this->mutex_.lock();
		vx = this->vx_;
		vy = this->vy_;
		this->mutex_.unlock();


		this->serial_->Lock();
		printf("Sending velocity [vx, vy]: %d, %d\n", vx, vy);
		this->WriteVelocity(vx, vy);

		this->serial_->UnLock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	printf("XY SERVICE IS NOT RUNNING\n\n");
}

void RNetServiceXY::SetVelocity(int8_t vx, int8_t vy) {
	this->mutex_.lock();
	this->vx_ = vx;
	this->vy_ = vy;
	this->mutex_.unlock();
}

void RNetServiceXY::WriteVelocity(int8_t vx, int8_t vy) {

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
	SeqNum = this->serial_->GetSequence();
	packet.SetHeader(SeqNum, 0, PacketType::DATAPACKET, MessageLength);
	packet.SetData(Message, MessageLength);

	printf("Send velocity...\n");
	// Write packet on serial port
	//this->WritePacket(packet);
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	//this->WritePacket(packet);
	do {
		this->serial_->WritePacket(packet);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		//printf("Data sent: ");
		packet.Dump();
	} while (this->serial_->WaitForAck(SeqNum, 20) == false);
	printf("Sent and Ack received\n");


	this->serial_->IncrementSequence();
}

}

#endif
