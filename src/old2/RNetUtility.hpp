#ifndef RNET_UTILITY_HPP
#define RNET_UTILITY_HPP

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include "RNetDefinitions.hpp"

namespace rnetserial {

class RNetUtility {
	public:
		static bool GetStartupFlag(const std::vector<uint8_t>& raw);
		static uint8_t GetSequenceNumber(const std::vector<uint8_t>& raw);
		static uint8_t GetDescriptor(const std::vector<uint8_t>& raw);
		static uint8_t GetDataLength(const std::vector<uint8_t>& raw);
		static uint8_t GetHdrChecksum(const std::vector<uint8_t>& raw);

		static bool IsHeaderValid(const std::vector<uint8_t>& raw);
		static bool IsDataValid(const std::vector<uint8_t>& raw);

		static void CRC8(uint8_t* pChecksum, const uint8_t* pBlock, uint32_t size);
		static void CRC16(uint16_t* pChecksum, const uint8_t* pBlock, uint32_t size);

};

}

#endif
