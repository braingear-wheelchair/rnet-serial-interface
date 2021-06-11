#ifndef RNETSERIAL_TX_CPP
#define RNETSERIAL_TX_CPP

#include "RNetSerialTx.hpp"


namespace rnetserial {

RNetSerialTx::RNetSerialTx(RNetSerial* serial, RNetBuffer* TxBuffer) {

	this->serial_ = serial;
	this->tx_     = TxBuffer;
}

RNetSerialTx::~RNetSerialTx(void) {
}

void RNetSerialTx::Run(void) {

	RNetPacket PktRx;
	
	printf("Serial TX SERVICE IS RUNNING\n\n");

	while(this->IsRunning()) {


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
