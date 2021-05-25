#ifndef SERIAL_INTERFACE_CPP
#define SERIAL_INTERFACE_CPP

#include "SerialInterface.hpp"


SerialInterface::SerialInterface(std::string name) {
	this->name_ = name;
}

SerialInterface::~SerialInterface(void) {
}

int SerialInterface::Open(std::string port) {

	this->serial_port_ = port;
	this->serial_handle_ = open(port.c_str(), O_RDWR);

	return this->serial_handle_;
}

int SerialInterface::Setup(void) {

	if(tcgetattr(this->serial_handle_, &(this->tty_)) != 0) {
		return -1;
	}

	this->tty_.c_cflag &= ~PARENB;			// Clear parity bit, disabling parity (most common)
  	this->tty_.c_cflag &= ~CSTOPB;			// Clear stop field, only one stop bit used in communication (most common)
  	this->tty_.c_cflag &= ~CSIZE;			// Clear all bits that set the data size 
  	this->tty_.c_cflag |= CS8;				// 8 bits per byte (most common)
  	this->tty_.c_cflag &= ~CRTSCTS;			// Disable RTS/CTS hardware flow control (most common)
  	this->tty_.c_cflag |= CREAD | CLOCAL;	// Turn on READ & ignore ctrl lines (CLOCAL = 1)

  	this->tty_.c_lflag &= ~ICANON;
  	this->tty_.c_lflag &= ~ECHO;			// Disable echo
  	this->tty_.c_lflag &= ~ECHOE;			// Disable erasure
  	this->tty_.c_lflag &= ~ECHONL;			// Disable new-line echo
  	this->tty_.c_lflag &= ~ISIG;			// Disable interpretation of INTR, QUIT and SUSP
  	this->tty_.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  	this->tty_.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	this->tty_.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  	this->tty_.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

	this->tty_.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  	this->tty_.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 115200
  	cfsetispeed(&(this->tty_), B115200);
  	cfsetospeed(&(this->tty_), B115200);

	// Save tty settings, also checking for error
  	if (tcsetattr(this->serial_handle_, TCSANOW, &(this->tty_)) != 0) {
  	    return -2;
  	}

	return 0;

}

ssize_t SerialInterface::Write(std::string msg) {
	
	unsigned char imsg[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\r' };
	printf("Size message: %ld\n", sizeof(imsg));
	return write(this->serial_handle_, imsg, sizeof(imsg));
}


void SerialInterface::Close(void) {
	close(this->serial_handle_);
}

std::string SerialInterface::name(void) {
	return this->name_;
}


#endif
