#ifndef RNETBUFFER_CPP
#define RNETBUFFER_CPP

#include "RNetBuffer.hpp"

namespace rnetserial {

RNetBuffer::RNetBuffer(void) {
}

RNetBuffer::~RNetBuffer(void) {
}

void RNetBuffer::Add(RNetPacket packet) {
	this->buffer_.push_back(packet);
}

void RNetBuffer::Remove(unsigned int index) {
	this->buffer_.erase(this->buffer_.begin() + index);
}

void RNetBuffer::Remove(const std::vector<RNetPacket>::iterator it) {
	this->buffer_.erase(it);
}

void RNetBuffer::Clear(void) {
	this->buffer_.clear();
}

size_t RNetBuffer::Size(void) {
	return this->buffer_.size();
}

void RNetBuffer::Lock(void) {
	this->mutex_.lock();
}

void RNetBuffer::Unlock(void) {
	this->mutex_.unlock();
}

std::vector<RNetPacket>::iterator RNetBuffer::Begin(void) {
	return this->buffer_.begin();
}

std::vector<RNetPacket>::iterator RNetBuffer::End(void) {
	return this->buffer_.end();
}

std::vector<RNetPacket>::const_iterator RNetBuffer::Begin(void) const {
	return this->buffer_.begin();
}

std::vector<RNetPacket>::const_iterator RNetBuffer::End(void) const {
	return this->buffer_.end();
}

std::vector<RNetPacket>::const_iterator RNetBuffer::cBegin(void) const {
	return this->buffer_.cbegin();
}

std::vector<RNetPacket>::const_iterator RNetBuffer::cEnd(void) const {
	return this->buffer_.cend();
}

}



#endif
