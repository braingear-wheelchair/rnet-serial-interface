#ifndef RNETSERVICE_XY_HPP
#define RNETSERVICE_XY_HPP

#include "RNetDefinitions.hpp"
#include "RNetSerial.hpp"
#include "RNetService.hpp"
#include "RNetBuffer.hpp"
#include "RNetTimer.hpp"

namespace rnetserial {

class RNetServiceXY : public RNetService {
	public:
		RNetServiceXY(RNetBuffer* TxBuffer, RNetBuffer* RxBuffer);
		~RNetServiceXY(void);

		void Run(void);

		void SetVelocity(int8_t vx, int8_t vy);
		bool WaitForAck(uint8_t SeqNum, unsigned int timeout = RNETCOMM_ACKTIMEOUT);

	private:
		RNetBuffer* tx_;
		RNetBuffer* rx_;
		RNetSerial* serial_;
		std::vector<uint8_t> vmsg_;
		std::mutex mutex_;
};


}


#endif
