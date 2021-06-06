#ifndef RNETSERIAL_HPP
#define RNETSERIAL_HPP

#include <string>
#include <unistd.h>
#include <iostream>
#include <exception>
#include <mutex>
#include <thread>
#include <libserial/SerialPort.h>

#include "RNetPacket.hpp"
#include "RNetTimer.hpp"

#define ACK_LENGTH	4  // bytes
#define ACK_TIMEOUT	0  // milliseconds

namespace rnetserial {


class RNetSerial {
	public:
		RNetSerial(const std::string name = "rnetserial");
		~RNetSerial(void);

		bool Open(const std::string port);
		bool SetDefaultParameters(void);
		bool SetBaudRate(const LibSerial::BaudRate& baudrate);
		bool SetCharacterSize(const LibSerial::CharacterSize& charactersize);
		bool SetFlowControl(const LibSerial::FlowControl& flowcontrol);
		bool SetParity(const LibSerial::Parity& parity);
		bool SetStopBits(const LibSerial::StopBits& stopbits);

		bool ReadPacket(RNetPacket& packet);
	
		bool WaitForAck(uint8_t SeqNum, int timeout = 20);	

		bool Connect(void);
		bool SendVelocity(int8_t vx, int8_t vy);
		bool ReadPacket(RNetPacket& packet, size_t NBytes, size_t Timeout);
		void Close(void);
		void Shutdown(void);
	
		void SetSequence(uint8_t SeqNum);
		uint8_t GetSequence(void);

		const std::string name(void);
		bool WritePacket(RNetPacket& packet);

		void Lock();
		void UnLock();
	protected:
		void IncrementSequence(void);

	private:
		std::string port_;
		std::string name_;

		LibSerial::SerialPort	 serialport_;
		LibSerial::BaudRate		 baudrate_;
		LibSerial::CharacterSize charactersize_;
		LibSerial::FlowControl	 flowcontrol_;
		LibSerial::Parity		 parity_;
		LibSerial::StopBits		 stopbits_;

		uint8_t sequence_number_;

		std::mutex	mutex_;

};


}



#endif
