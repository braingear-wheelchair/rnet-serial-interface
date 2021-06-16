#ifndef RNETSERVICE_RX_CPP
#define RNETSERVICE_RX_CPP

#include "RNetServiceRx.hpp"

namespace rnet {

RNetServiceRx::RNetServiceRx(RNetBuffer* TxBuffer, RNetBuffer* RxBuffer) {
	this->name_ = "rnet_service_receiver";
	this->tx_ = TxBuffer;
	this->rx_ = RxBuffer;
}

RNetServiceRx::~RNetServiceRx(void) {
}

void RNetServiceRx::Run(void) {

	RNetPacket AckTx;
	std::vector<uint8_t> ackdata;
	std::vector<uint8_t> todelete;

	printf("[%s] Service is up\n", this->name().c_str());
	while(this->IsRunning()) {

		// Clear local list of element to be deleted
		todelete.clear();

		// Lock the Rx Buffer
		this->rx_->Lock();

		// Iterate for every element of the Rx Buffer
		for(auto it=this->rx_->Begin(); it != this->rx_->End(); it++) {
	
			// If the element has DATAPACKET type then add a ACK packet to Tx
			// Buffer
			if(it->GetType() == PacketType::DATAPACKET) {

				// Set the AckPacket with the SeqNumber of the Rx packet
				AckTx.Set(it->GetSeqNum(), PacketType::ACKPACKET, ackdata, 0);

				// Add the AckPacket to the Tx Buffer
				this->tx_->Lock();
				this->tx_->Add(AckTx);
				this->tx_->Unlock();

				// Add the index of this packet to the list to be deleted
				todelete.push_back(it - this->rx_->Begin());
			}

		}

		// Delete all the packet from the Rx Buffer that have been already
		// acknowledged
		for (auto it=todelete.begin(); it != todelete.end(); it++)
			this->rx_->Remove(*it);

		// Unlock the Rx Buffer
		this->rx_->Unlock();
	}

	printf("[%s] Service is down\n", this->name().c_str());

}


}

#endif
