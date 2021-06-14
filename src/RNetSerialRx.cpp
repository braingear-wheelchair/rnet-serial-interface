#ifndef RNETSERIAL_RX_CPP
#define RNETSERIAL_RX_CPP

#include "RNetSerialRx.hpp"


namespace rnetserial {

RNetSerialRx::RNetSerialRx(RNetSerial* serial, RNetBuffer* RxBuffer) {

	this->serial_ = serial;
	this->rx_     = RxBuffer;
}

RNetSerialRx::~RNetSerialRx(void) {
}

void RNetSerialRx::Run(void) {

	RNetPacket PktRx;

	bool isnew;
	printf("Serial RX SERVICE IS RUNNING\n\n");

	while(this->IsRunning() && this->serial_->IsOpen()) {


		this->serial_->Lock();
		isnew = this->serial_->ReadPacket(PktRx);
		this->serial_->Unlock();

		if (isnew == true) {
			//printf("Received packet:\n");
			//PktRx.DumpRaw();
			this->rx_->Lock();
			this->rx_->Add(PktRx);
			this->rx_->Unlock();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));


	}
	printf("Serial RX SERVICE IS NOT RUNNING\n\n");
	

}

}


#endif
