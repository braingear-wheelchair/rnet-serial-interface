#ifndef RNETRECEIVER_CPP
#define RNETRECEIVER_CPP

#include "RNetReader.hpp"


namespace rnet {

RNetReader::RNetReader(RNetSerial* serial, RNetBuffer* RxBuffer) {

	this->name_   = "rnet_reader";
	this->serial_ = serial;
	this->rx_     = RxBuffer;
}

RNetReader::~RNetReader(void) {
}

void RNetReader::Run(void) {

	RNetPacket PktRx;

	bool isnew;
	
	printf("[%s] Service is up\n", this->name().c_str());

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

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
	printf("[%s] Service is down\n", this->name().c_str());
	

}

}


#endif
