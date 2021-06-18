#ifndef RNETWRITER_HPP
#define RNETWRITER_HPP

#include "RNetSerial.hpp"
#include "RNetService.hpp"
#include "RNetBuffer.hpp"

namespace rnet {

class RNetWriter : public RNetService {
	public:
		RNetWriter(RNetSerial* serial, RNetBuffer* TxBuffer);
		~RNetWriter(void);

		void Run(void);

	private:
		RNetSerial* serial_;
		RNetBuffer* tx_;


};


}


#endif
