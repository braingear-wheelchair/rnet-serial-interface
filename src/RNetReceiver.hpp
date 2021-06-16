#ifndef RNETRECEIVER_HPP
#define RNETRECEIVER_HPP

#include "RNetSerial.hpp"
#include "RNetService.hpp"
#include "RNetBuffer.hpp"

namespace rnet {

class RNetReceiver : public RNetService {
	public:
		RNetReceiver(RNetSerial* serial, RNetBuffer* RxBuffer);
		~RNetReceiver(void);

		void Run(void);

	private:
		RNetSerial* serial_;
		RNetBuffer* rx_;


};


}


#endif
