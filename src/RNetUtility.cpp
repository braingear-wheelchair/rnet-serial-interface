#ifndef RNET_UTILITY_CPP
#define RNET_UTILITY_CPP

#include "RNetUtility.hpp"

namespace rnet {

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

bool RNetUtility::IsHeaderValid(const std::vector<uint8_t>& raw) {

	uint8_t  hchecksum;
	uint8_t  hdata[2];

	// Check dimension
	if(raw.size() < 4)
		return false;
	
	hdata[0] = raw.at(1);
	hdata[1] = raw.at(2);

	// Check SYS byte
	if(raw.at(0) != RNETPACKET_SYN_DEFAULT)
		return false;

	// Check header checksum
	RNetChecksum::CRC8(&hchecksum, hdata, 2);	
	if(raw.at(3) != hchecksum)
		return false;

	return true;
}

bool RNetUtility::IsDataValid(const std::vector<uint8_t>& raw) {

	uint8_t  DataLength;
	uint8_t* pdata;
	uint16_t checksum;
	uint16_t rchecksum;

	// Get Data dimension
	DataLength = raw.size();

	// If data size is 0, no packet (we assume that is valid)
	if(DataLength == 0)
		return true;

	// Checksum of the data
	pdata = new uint8_t[DataLength-2];
	for(auto i=0; i<DataLength-2; i++)
		pdata[i] = raw.at(i);

	rchecksum = ((uint16_t)raw.at(DataLength-2) << 8) | raw.at(DataLength-1);	
	RNetChecksum::CRC16(&checksum, pdata, DataLength - 2); 
	
	// destroy the array
	delete pdata;

	if(rchecksum != checksum)
		return false;

	return true;
}


}

#endif
