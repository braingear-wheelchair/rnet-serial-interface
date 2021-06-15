#ifndef RNETSERVICE_RX_HPP
#define RNETSERVICE_RX_HPP

#include "RNetService.hpp"
#include "RNetBuffer.hpp"

namespace rnet {

class RNetServiceRx : public RNetService {
	public:
		RNetServiceRx(RNetBuffer* TxBuffer, RNetBuffer* RxBuffer);
		~RNetServiceRx(void);

		void Run(void);

	private:
		RNetBuffer* tx_;
		RNetBuffer* rx_;
};


}


#endif
