#ifndef RNETCOUNTER_CPP
#define RNETCOUNTER_CPP

#include "RNetCounter.hpp"

namespace rnetserial {

RNetCounter::RNetCounter(void) {
	this->counter_ = 0;;
}

RNetCounter& RNetCounter::Instance(void) {
	static RNetCounter instance;
	return instance;
}

void RNetCounter::Increment(void) {
	this->counter_++;
	if(this->counter_ > 31)
		this->counter_ = 0;
}

void RNetCounter::Decrement(void) {
	this->counter_--;
	if(this->counter_ < 0)
		this->counter_ = 31;
}

uint8_t RNetCounter::Get(void) {
	return this->counter_;
}

void RNetCounter::Set(uint8_t value) {
	this->counter_ = value;
}

void RNetCounter::Reset(void) {
	this->counter_ = 0;
}

}



#endif
