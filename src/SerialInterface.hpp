#ifndef SERIAL_INTERFACE_HPP
#define SERIAL_INTERFACE_HPP


// C/C++ library headers
#include <stdio.h>
#include <string>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

class SerialInterface {
	public:
		SerialInterface(std::string name = "serial_interface");
		~SerialInterface(void);

		int Open(std::string port);
		void Close(void);

		int Setup(void);

		ssize_t Write(std::string msg);
		std::string name(void);




	public:
		std::string serial_port_;
		int serial_handle_;
		struct termios tty_;
		std::string name_;





};





#endif
