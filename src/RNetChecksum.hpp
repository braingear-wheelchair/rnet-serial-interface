#ifndef RNET_CHECKSUM_HPP
#define RNET_CHECKSUM_HPP

#include <string>

#define CHECKSUM_CRC16_INIT_VALUE	0xFFFFu
#define CHECKSUM_CRC16_POLY			0x90D9u
#define CHECKSUM_CRC8_INIT_VALUE	0xFFu
#define CHECKSUM_CRC8_POLY			0x2Fu

namespace rnetserial {

class RNetChecksum {
	public:
		static void CRC8(uint8_t* pChecksum, const uint8_t* pBlock, uint32_t size);
		static void CRC16(uint16_t* pChecksum, const uint8_t* pBlock, uint32_t size);
};

}

#endif
