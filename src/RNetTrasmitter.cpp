#ifndef RNETTRASMITTER_CPP
#define RNETTRASMITTER_CPP

#include "RNetTrasmitter.hpp"


namespace rnet {

RNetTrasmitter::RNetTrasmitter(RNetSerial* serial, RNetBuffer* TxBuffer) {

	this->serial_ = serial;
	this->tx_     = TxBuffer;
}

RNetTrasmitter::~RNetTrasmitter(void) {
}

void RNetTrasmitter::Run(void) {

	RNetPacket PktRx;
	
	printf("Serial TX SERVICE IS RUNNING\n\n");

	while(this->IsRunning() && this->serial_->IsOpen()) {


		this->serial_->Lock();

		this->tx_->Lock();

		//if (this->tx_->Size() > 0) {
		//	printf("Sending the following packets:\n");
		//}

		for(auto it=this->tx_->Begin(); it != this->tx_->End(); it++) {
			//it->DumpRaw();
			this->serial_->WritePacket(*it);
		}
	
		this->tx_->Clear();
		this->tx_->Unlock();
		
		this->serial_->Unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));


	}
	printf("Serial TX SERVICE IS NOT RUNNING\n\n");
	

}

}


#endif
