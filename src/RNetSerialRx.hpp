#ifndef RNETSERIAL_RX_HPP
#define RNETSERIAL_RX_HPP

#include "RNetSerial.hpp"
#include "RNetService.hpp"
#include "RNetBuffer.hpp"

namespace rnetserial {

class RNetSerialRx : public RNetService {
	public:
		RNetSerialRx(RNetSerial* serial, RNetBuffer* RxBuffer);
		~RNetSerialRx(void);

		void Run(void);

	private:
		RNetSerial* serial_;
		RNetBuffer* rx_;


};


}


#endif
