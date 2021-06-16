#ifndef RNETSERVICE_KEYBOARD_HPP
#define RNETSERVICE_KEYBOARD_HPP

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stropts.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "RNetService.hpp"

namespace rnetserial {

class RNetServiceKeyboard : public RNetService {
	public:
		RNetServiceKeyboard(void);
		~RNetServiceKeyboard(void);

		void GetKey(int& key);
		void Run(void);

	private:
		int kbhit(void);
		int getch(void);
		void enable_raw_mode(void);
		void disable_raw_mode(void);
	private:
		bool run_;
		int peek_character;
		int key_;
		termios term_;

};


}
#endif
