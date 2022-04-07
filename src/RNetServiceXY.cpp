#ifndef RNETSERVICE_XY_CPP
#define RNETSERVICE_XY_CPP

#include "RNetServiceXY.hpp"

namespace rnet {

RNetServiceXY::RNetServiceXY(RNetBuffer* TxBuffer, RNetBuffer* RxBuffer) {
	this->name_ = "rnet_service_xy";
	this->tx_ = TxBuffer;
	this->rx_ = RxBuffer;
	this->first_ = false;
	//this->SetVelocity(0, 0);
}

RNetServiceXY::~RNetServiceXY(void) {
}

void RNetServiceXY::Run(void) {

	RNetPacket Vxy;
	std::vector<uint8_t> vmsg;
	uint8_t SeqNum;
	bool pass = false;

	printf("[%s] Service is up\n", this->name().c_str());
	while(this->IsRunning()) {

		this->mutex_.lock();
		if(this->first_ == false) {
			pass = true;
		}
		this->mutex_.unlock();

		if (pass == true) {
			pass = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

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
	
	printf("[%s] Service is down\n", this->name().c_str());

}

void RNetServiceXY::SetVelocity(int8_t vx, int8_t vy) {

	this->mutex_.lock();
	this->first_ = true;
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


