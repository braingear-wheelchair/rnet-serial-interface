#ifndef RNET_UTILITY_CPP
#define RNET_UTILITY_CPP

#include "RNetUtility.hpp"

namespace rnetserial {

bool RNetUtility::GetStartupFlag(const std::vector<uint8_t>& raw) {
	if(raw.size() < RNETPACKET_SIZE_HEADER) {
		throw std::runtime_error("Error - Wrong header size");
	}

	return (raw.at(1) & 0x4) >> 2;
}

uint8_t RNetUtility::GetSequenceNumber(const std::vector<uint8_t>& raw) {
	if(raw.size() < RNETPACKET_SIZE_HEADER) {
		throw std::runtime_error("Error - Wrong header size");
	}

	return (raw.at(1) & 0xF8) >> 3;
}

uint8_t RNetUtility::GetDescriptor(const std::vector<uint8_t>& raw) {
	if(raw.size() < RNETPACKET_SIZE_HEADER) {
		throw std::runtime_error("Error - Wrong header size");
	}

	return (raw.at(1) & 0x3);
}

uint8_t RNetUtility::GetDataLength(const std::vector<uint8_t>& raw) {
	if(raw.size() < RNETPACKET_SIZE_HEADER) {
		throw std::runtime_error("Error - Wrong header size");
	}

	return raw.at(2);
}

uint8_t RNetUtility::GetHdrChecksum(const std::vector<uint8_t>& raw) {
	if(raw.size() < RNETPACKET_SIZE_HEADER) {
		throw std::runtime_error("Error - Wrong header size");
	}

	return raw.at(3);
}

void RNetUtility::CRC8(uint8_t* pChecksum, const uint8_t* pBlock, uint32_t size) {
	
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

void RNetUtility::CRC16(uint16_t* pChecksum, const uint8_t * pBlock, uint32_t size) {
	
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
