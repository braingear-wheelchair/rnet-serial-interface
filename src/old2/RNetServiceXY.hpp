#ifndef RNETSERVICE_XY_HPP
#define RNETSERVICE_XY_HPP

#include "RNetDefinitions.hpp"
#include "RNetService.hpp"
#include "RNetSerial.hpp"
#include "RNetPacket.hpp"
#include "RNetUtility.hpp"

namespace rnetserial {

class RNetServiceXY : public RNetService {

	public:
		RNetServiceXY(RNetSerial* serial);
		~RNetServiceXY(void);

		void Run(void);

		void SetVelocity(int8_t vx, int8_t vy);
		
	protected:
		void WriteVelocity(int8_t vx, int8_t vy);

	protected:
		RNetSerial* serial_;
		int8_t vx_;
		int8_t vy_;

	private:
		std::mutex mutex_;


};

}

#endif
