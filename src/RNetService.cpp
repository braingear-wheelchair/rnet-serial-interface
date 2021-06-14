#ifndef RNETSERVICE_CPP
#define RNETSERVICE_CPP

#include "RNetService.hpp"

namespace rnetserial {

RNetService::RNetService(void) {
	this->mutex_.lock();
	this->run_ = false;
	this->mutex_.unlock();
}

RNetService::~RNetService(void) {
	//bool isrunning = this->IsRunning();
	//if(isrunning == true) {
	//	this->Stop();
	//}
	//this->Stop();
	//this->Join();
}

void RNetService::Start(void) {

	if(this->IsRunning() == true) {
		this->Stop();
		this->Join();
	}

	this->mutex_.lock();
	this->thread_ = std::thread(&RNetService::Run, this);
	this->run_ = true;
	this->mutex_.unlock();
	
}

void RNetService::Stop(void) {
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
	this->mutex_.lock();
	this->thread_.join();
	this->mutex_.unlock();
}

}


#endif
