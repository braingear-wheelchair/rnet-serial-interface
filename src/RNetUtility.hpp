#ifndef RNET_UTILITY_HPP
#define RNET_UTILITY_HPP

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include "RNetDefinitions.hpp"
#include "RNetChecksum.hpp"

namespace rnet {

class RNetUtility {
	public:
		static bool GetStartupFlag(const std::vector<uint8_t>& raw);
		static uint8_t GetSequenceNumber(const std::vector<uint8_t>& raw);
		static uint8_t GetDescriptor(const std::vector<uint8_t>& raw);
		static uint8_t GetDataLength(const std::vector<uint8_t>& raw);
		static uint8_t GetHdrChecksum(const std::vector<uint8_t>& raw);

		static bool IsHeaderValid(const std::vector<uint8_t>& raw);
		static bool IsDataValid(const std::vector<uint8_t>& raw);

};

}

#endif
