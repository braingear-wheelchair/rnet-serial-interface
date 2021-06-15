#ifndef RNETTRASMITTER_HPP
#define RNETTRASMITTER_HPP

#include "RNetSerial.hpp"
#include "RNetService.hpp"
#include "RNetBuffer.hpp"

namespace rnet {

class RNetTrasmitter : public RNetService {
	public:
		RNetTrasmitter(RNetSerial* serial, RNetBuffer* TxBuffer);
		~RNetTrasmitter(void);

		void Run(void);

	private:
		RNetSerial* serial_;
		RNetBuffer* tx_;


};


}


#endif
