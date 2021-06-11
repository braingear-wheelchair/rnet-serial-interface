#ifndef RNETSERIAL_TX_HPP
#define RNETSERIAL_TX_HPP

#include "RNetSerial.hpp"
#include "RNetService.hpp"
#include "RNetBuffer.hpp"

namespace rnetserial {

class RNetSerialTx : public RNetService {
	public:
		RNetSerialTx(RNetSerial* serial, RNetBuffer* TxBuffer);
		~RNetSerialTx(void);

		void Run(void);

	private:
		RNetSerial* serial_;
		RNetBuffer* tx_;


};


}


#endif
