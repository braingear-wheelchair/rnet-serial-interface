#ifndef RNET_CHECKSUM_CPP
#define RNET_CHECKSUM_CPP

#include "RNetChecksum.hpp"


namespace rnetserial {

void RNetChecksum::CRC8(uint8_t* pChecksum, const uint8_t* pBlock, uint32_t size) {
	
	uint8_t Temp;
	uint8_t Flag;
	
	*(pChecksum) = CHECKSUM_CRC8_INIT_VALUE;
	
	while (size > 0u) {
		Temp = *pBlock++;
		
		for (auto i=0; i<8u; i++) {
			Flag = (uint8_t)(*pChecksum ^ Temp) & 0x80u;
			*pChecksum <<= 1u;
			Temp <<= 1u;
			if (Flag) {
				*pChecksum ^= CHECKSUM_CRC8_POLY;
			}
		}
		size--;
	}
}

void RNetChecksum::CRC16(uint16_t* pChecksum, const uint8_t * pBlock, uint32_t size) {
	
	uint8_t Temp;
	uint8_t Flag;
	
	*(pChecksum) = CHECKSUM_CRC16_INIT_VALUE;
	
	while (size > 0u) {
		Temp = *pBlock++;
		for (auto i=0u; i<8u; i++) {
			Flag = ((uint8_t)(*pChecksum >> 8u) ^ Temp) & 0x80u;
			*pChecksum <<= 1u;
			Temp <<= 1u;
			if (Flag) {
				*pChecksum ^= CHECKSUM_CRC16_POLY;
			}
		}
		size--;
	}
}



}

#endif
