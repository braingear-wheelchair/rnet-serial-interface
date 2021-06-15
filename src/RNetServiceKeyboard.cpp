#ifndef RNETSERVICE_KEYBOARD_CPP
#define RNETSERVICE_KEYBOARD_CPP

#include "RNetServiceKeyboard.hpp"


namespace rnet {

RNetServiceKeyboard::RNetServiceKeyboard(void) {
	int flags = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flags | O_NONBLOCK); 
}

RNetServiceKeyboard::~RNetServiceKeyboard(void) {
	int flags = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flags | ~O_NONBLOCK); 
	//this->disable_raw_mode();
	//tcflush(0, TCIFLUSH);
}

void RNetServiceKeyboard::Run(void) {

	char key;
	bool isrunning = true;
	printf("KEY SERVICE IS RUNNING\n\r");
	this->kmutex_.lock();
	this->enable_raw_mode();
	this->kmutex_.unlock();
	
	while(isrunning == true) {

		this->kmutex_.lock();
		if(this->kbhit()) {
			this->key_ = this->getch();
			//printf("%c\n", this->getch());
		}
		this->kmutex_.unlock();
		
		
		isrunning = this->IsRunning();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	printf("\nOut while\n");
	this->kmutex_.lock();
	this->disable_raw_mode();
	this->kmutex_.unlock();
	tcflush(0, TCIFLUSH);
	tcflush(0, TCOFLUSH);
	
	printf("KEY SERVICE IS NOT RUNNING\n");

}

void RNetServiceKeyboard::GetKey(int& key) {

	this->kmutex_.lock();
	if(this->key_ > 0) {	
		key = this->key_;
	} else { 
		this->key_ = -1;
		key = -1;
	}
	this->kmutex_.unlock();
}

int RNetServiceKeyboard::kbhit(void) {
	
	unsigned char ch;
    int nread;
	
	if (peek_character != -1) {
		return 1;
	}

    nread = read(0,&ch,1);

    if (nread == 1){
        peek_character = ch;
        return 1;
    }
    return 0;
}

int RNetServiceKeyboard::getch(){
    char ch;
	int nread;

    if (peek_character != -1){
        ch = peek_character;
        peek_character = -1;
	}
    //} else { 
	//	nread = read(0,&ch,1);
	//}

    return ch;
}

void RNetServiceKeyboard::enable_raw_mode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
	//termios term;
    //tcgetattr(0, &(this->term_));
	//term = this->term_;
    //term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    //tcsetattr(0, TCSANOW, &(term));
}

void RNetServiceKeyboard::disable_raw_mode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
    //this->term_.c_lflag |= ICANON | ECHO;
	//tcsetattr(0, TCSANOW, &(this->term_));
}



}

#endif
