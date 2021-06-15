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
#include "RNetCounter.hpp"


namespace rnet {


class RNetSerial : protected LibSerial::SerialPort {
	public:
		RNetSerial(const std::string name = "rnet");
		~RNetSerial(void);

		bool Open(const std::string port);
		void Close(void);
		bool Connect(int timeout = -1);
		bool IsOpen(void);


		bool WritePacket(RNetPacket& packet);
		bool ReadPacket(RNetPacket& packet);
		

		void Shutdown(void);
	

		const std::string name(void);

		void Lock();
		void Unlock();
	protected:

	private:
		std::string port_;
		std::string name_;
		std::mutex	mutex_;

};


}



#endif
