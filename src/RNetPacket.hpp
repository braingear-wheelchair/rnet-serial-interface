#ifndef RNETPACKET_HPP
#define RNETPACKET_HPP

#include <vector>
#include <string>

#include "RNetDefinitions.hpp"
#include "RNetChecksum.hpp"

namespace rnet {

enum PacketType {ACKPACKET, NACKPACKET, DATAPACKET};

class RNetPacket {
	public:
		RNetPacket(void);
		~RNetPacket(void);

		void Set(uint8_t SeqNum, uint8_t Type, std::vector<uint8_t> Data, uint8_t DataLength, bool Startup = 0);

		void	 SetStartupFlag(bool startup);
		void	 SetSeqNum(uint8_t seqnum);
		void	 SetType(uint8_t type);
		void	 SetData(std::vector<uint8_t> data, uint8_t datalength);

		bool	 GetStartupFlag(void);
		uint8_t  GetSeqNum(void);
		uint8_t  GetType(void);
		uint8_t  GetDataLength(void);
		std::vector<uint8_t> GetData(void);

		bool IsType(uint8_t type);
		bool IsSeqNum(uint8_t seqnum);
		bool IsStartup(void);

		std::vector<uint8_t> EncodeHeader(void);
		std::vector<uint8_t> EncodeData(void);
		std::vector<uint8_t> Encode(void);

		void DecodeHeader(std::vector<uint8_t> header);
		void DecodeData(std::vector<uint8_t> data);
		void Decode(std::vector<uint8_t> packet);

		void DumpRaw(void);
		void Dump(void);

	private:
		bool				 startup_;
		uint8_t				 seqnum_;
		uint8_t 			 type_;
		uint8_t 			 datalength_;
		std::vector<uint8_t> data_;


};

};


#endif
