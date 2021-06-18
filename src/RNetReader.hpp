#ifndef RNETREADER_HPP
#define RNETREADER_HPP

#include "RNetSerial.hpp"
#include "RNetService.hpp"
#include "RNetBuffer.hpp"

namespace rnet {

class RNetReader : public RNetService {
	public:
		RNetReader(RNetSerial* serial, RNetBuffer* RxBuffer);
		~RNetReader(void);

		void Run(void);

	private:
		RNetSerial* serial_;
		RNetBuffer* rx_;


};


}


#endif
