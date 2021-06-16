#ifndef RNETTRASMITTER_CPP
#define RNETTRASMITTER_CPP

#include "RNetTrasmitter.hpp"


namespace rnet {

RNetTrasmitter::RNetTrasmitter(RNetSerial* serial, RNetBuffer* TxBuffer) {

	this->name_   = "rnet_trasmitter";
	this->serial_ = serial;
	this->tx_     = TxBuffer;
}

RNetTrasmitter::~RNetTrasmitter(void) {
}

void RNetTrasmitter::Run(void) {

	RNetPacket PktRx;
	
	printf("[%s] Service is up\n", this->name().c_str());

	while(this->IsRunning() && this->serial_->IsOpen()) {


		this->serial_->Lock();

		this->tx_->Lock();

		// Firstly, it consumes all the ACK packets
		for(auto it=this->tx_->Begin(); it != this->tx_->End(); it++) {
			if(it->GetType() == ACKPACKET)
				this->serial_->WritePacket(*it);
		}
		
		// Secondly, it consumes all the DATA packets
		for(auto it=this->tx_->Begin(); it != this->tx_->End(); it++) {
			if(it->GetType() == DATAPACKET)
				this->serial_->WritePacket(*it);
		}
	
		this->tx_->Clear();
		this->tx_->Unlock();
		
		this->serial_->Unlock();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}
	printf("[%s] Service is down\n", this->name().c_str());
	

}

}


#endif
