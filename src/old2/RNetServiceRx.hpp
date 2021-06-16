#ifndef RNETSERVICE_RX_HPP
#define RNETSERVICE_RX_HPP

#include "RNetDefinitions.hpp"
#include "RNetService.hpp"
#include "RNetSerial.hpp"
#include "RNetPacket.hpp"
#include "RNetUtility.hpp"

namespace rnetserial {

class RNetServiceRx : public RNetService {

	public:
		RNetServiceRx(RNetSerial* serial);
		~RNetServiceRx(void);

		void Run(void);


	protected:
		RNetSerial* serial_;

};

}

#endif
