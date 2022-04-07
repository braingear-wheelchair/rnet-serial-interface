#ifndef RNETSERVICE_CPP
#define RNETSERVICE_CPP

#include "RNetService.hpp"

namespace rnet {

RNetService::RNetService(void) {
	this->mutex_.lock();
	this->run_ = false;
	this->mutex_.unlock();
}

RNetService::~RNetService(void) {
	if(this->IsRunning() == true) {
		this->Stop();
		this->Join();
	}
}

void RNetService::Start(void) {

	if(this->IsRunning() == true) {
		this->Stop();
		this->Join();
	}

	printf("[%s] Service is requested to start\n", this->name().c_str());
	this->thread_ = std::thread(&RNetService::Run, this);
	this->mutex_.lock();
	this->run_ = true;
	this->mutex_.unlock();
	
}

void RNetService::Stop(void) {
	printf("[%s] Service is requested to stop\n", this->name().c_str());
	this->mutex_.lock();
	this->run_ = false;
	this->mutex_.unlock();
}

bool RNetService::IsRunning(void) {
	bool ret = false;
	this->mutex_.lock();
	ret = this->run_;
	this->mutex_.unlock();
	return ret;
}

void RNetService::Join(void) {
	printf("[%s] Service is requested to join\n", this->name().c_str());
	if(this->thread_.joinable() == true)
		this->thread_.join();
}

std::string RNetService::name(void) {
	std::string name;
	this->mutex_.lock();
	name = this->name_;
	this->mutex_.unlock();
	return name;
}


}


#endif
