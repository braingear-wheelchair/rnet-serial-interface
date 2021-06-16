#ifndef RNETSERVICE_RX_CPP
#define RNETSERVICE_RX_CPP

#include "RNetServiceRx.hpp"

namespace rnetserial {

RNetServiceRx::RNetServiceRx(RNetSerial* serial) {
	this->serial_ = serial;
}

RNetServiceRx::~RNetServiceRx(void) {
}

void RNetServiceRx::Run(void) {

	RNetPacket AckTx;
	RNetPacket PktRx;
	
	while(this->IsRunning()) {

		this->serial_->Lock();
		if(this->serial_->ReadPacket(PktRx)) {
			PktRx.Dump();
			if(PktRx.GetHeader()->Descriptor != rnetserial::PacketType::ACKPACKET) {
				AckTx.SetHeader(PktRx.GetHeader()->SequenceNumber, 0, rnetserial::PacketType::ACKPACKET, 0);
				this->serial_->WritePacket(AckTx);
			}
		}
		this->serial_->UnLock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	printf("RX SERVICE IS NOT RUNNING\n\n");
}

}

#endif
