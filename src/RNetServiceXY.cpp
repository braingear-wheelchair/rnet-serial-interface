#ifndef RNETSERVICE_XY_CPP
#define RNETSERVICE_XY_CPP

#include "RNetServiceXY.hpp"

namespace rnetserial {

RNetServiceXY::RNetServiceXY(RNetBuffer* TxBuffer, RNetBuffer* RxBuffer) {
	this->tx_ = TxBuffer;
	this->rx_ = RxBuffer;
	this->SetVelocity(0, 0);
}

RNetServiceXY::~RNetServiceXY(void) {
}

void RNetServiceXY::Run(void) {

	RNetPacket Vxy;
	std::vector<uint8_t> vmsg;
	uint8_t SeqNum;

	printf("XY SERVICE IS RUNNING\n\n");
	while(this->IsRunning()) {

		
		this->mutex_.lock();
		vmsg = this->vmsg_;
		this->mutex_.unlock();

		SeqNum = RNetCounter::Instance().Get();
		
		Vxy.Set(SeqNum, PacketType::DATAPACKET, vmsg, vmsg.size());
		//Vxy.DumpRaw();
		this->tx_->Lock();
		this->tx_->Add(Vxy);
		this->tx_->Unlock();


		this->WaitForAck(SeqNum);
		
		RNetCounter::Instance().Increment();
	}
	
	printf("XY SERVICE IS NOT RUNNING\n\n");

}

void RNetServiceXY::SetVelocity(int8_t vx, int8_t vy) {

	this->mutex_.lock();
	this->vmsg_.clear();
	this->vmsg_.push_back(0x0b);
	this->vmsg_.push_back(0x00);
	this->vmsg_.push_back(0x00);
	this->vmsg_.push_back((uint8_t)vx);
	this->vmsg_.push_back((uint8_t)vy);
	this->mutex_.unlock();

}

bool RNetServiceXY::WaitForAck(uint8_t SeqNum, unsigned int timeout) {

	bool found = false;
	unsigned int index;

	timer_msecs timer;

	timer.tic();
	while(found == false && timer.toc() < timeout) {

		this->rx_->Lock();
		for(auto it = this->rx_->Begin(); it != this->rx_->End(); it++) {
			
			if(it->GetSeqNum() == SeqNum) {
				index = it - this->rx_->Begin();
				found = true;
				break;
			}
		}

		if (found == true) {
			this->rx_->Remove(index);
		}

		this->rx_->Unlock();

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return found;

}

}

#endif


