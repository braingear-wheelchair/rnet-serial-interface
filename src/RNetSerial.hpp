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
#include "RNetUtility.hpp"
//#include "RNetTimer.hpp"


namespace rnetserial {


class RNetSerial : protected LibSerial::SerialPort {
	public:
		RNetSerial(const std::string name = "rnetserial");
		~RNetSerial(void);

		bool OpenPort(const std::string port);
		void ClosePort(void);
		bool Connect(int timeout = -1);
		
		bool WritePacket(RNetPacket& packet);
		bool ReadPacket(RNetPacket& packet);
		
		//bool ReadPacket(RNetPacket& packet, size_t NBytes, size_t Timeout);

	
		//bool WaitForAck(uint8_t SeqNum, int timeout = RNETCOMM_ACKTIMEOUT);	

		bool SendVelocity(int8_t vx, int8_t vy);
		void Shutdown(void);
	
		void SetSequence(uint8_t SeqNum);
		uint8_t GetSequence(void);

		const std::string name(void);

		void Lock();
		void Unlock();
		void IncrementSequence(void);
	protected:

	private:
		std::string port_;
		std::string name_;

		uint8_t sequence_number_;

		std::mutex	mutex_;

};


}



#endif
