#ifndef RNETPACKET_CHECKSUM_HPP
#define RNETPACKET_CHECKSUM_HPP

#include <string>
#include "RNetDefinitions.hpp"

namespace rnetserial {

class RNetChecksum {
	public:
		static void CRC8(uint8_t* pChecksum, const uint8_t* pBlock, uint32_t size);
		static void CRC16(uint16_t* pChecksum, const uint8_t* pBlock, uint32_t size);

};

}

#endif
