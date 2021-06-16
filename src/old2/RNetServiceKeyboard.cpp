#ifndef RNETSERVICE_KEYBOARD_CPP
#define RNETSERVICE_KEYBOARD_CPP

#include "RNetServiceKeyboard.hpp"


namespace rnetserial {

RNetServiceKeyboard::RNetServiceKeyboard(void) {
}

RNetServiceKeyboard::~RNetServiceKeyboard(void) {
	this->disable_raw_mode();
	//tcflush(0, TCIFLUSH);
}

void RNetServiceKeyboard::Run(void) {

	char key;
	this->enable_raw_mode();
	while(this->IsRunning()) {
		
		if(this->kbhit()) {
			this->key_ = this->getch();
			//printf("%c\n", this->getch());
		}
		
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	printf("KEY SERVICE IS NOT RUNNING\n\r");
	this->disable_raw_mode();
	//tcflush(0, TCIFLUSH);
	//tcflush(0, TCOFLUSH);

}

void RNetServiceKeyboard::GetKey(int& key) {

	if(this->key_ > 0) {	
		key = this->key_;
	} else { 
		this->key_ = -1;
		key = -1;
	}
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
    } else { 
		nread = read(0,&ch,1);
	}

    return ch;
}

void RNetServiceKeyboard::enable_raw_mode() {
    //termios term;
    //tcgetattr(0, &term);
    //term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    //tcsetattr(0, TCSANOW, &term);
	termios term;
    tcgetattr(0, &(this->term_));
	term = this->term_;
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &(term));
}

void RNetServiceKeyboard::disable_raw_mode() {
    //termios term;
    //tcgetattr(0, &term);
    //term.c_lflag |= ICANON | ECHO;
    //tcsetattr(0, TCSANOW, &term);
    this->term_.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &(this->term_));
	printf("\r\n");
}



}

#endif
