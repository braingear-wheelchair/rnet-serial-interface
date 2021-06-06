#ifndef RNETSERVICE_RX_HPP
#define RNETSERVICE_RX_HPP

#include <chrono>
#include <thread>

#include "RNetDefinitions.hpp"
#include "RNetSerial.hpp"
#include "RNetPacket.hpp"
#include "RNetUtility.hpp"

namespace rnetserial {

class RNetServiceRx {

	public:
		RNetServiceRx(RNetSerial* serial);
		~RNetServiceRx(void);

		void Run(void);
		void Start(void);
		void Stop(void);


	private:

	private:
		bool run_;

	protected:
		RNetSerial* serial_;

};

}

#endif
