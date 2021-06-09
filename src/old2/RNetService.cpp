#ifndef RNETSERVICE_CPP
#define RNETSERVICE_CPP

#include "RNetService.hpp"

namespace rnetserial {

RNetService::RNetService(void) {
	this->run_ = false;
}

RNetService::~RNetService(void) {
	if(this->IsRunning() == true) {
		this->Stop();
	}
	this->Join();
}

void RNetService::Start(void) {

	if(this->IsRunning() == true) {
		this->Stop();
		this->Join();
	}

	this->thread_ = std::thread(&RNetService::Run, this);
	this->run_ = true;
	
}

void RNetService::Stop(void) {
	if(this->IsRunning() == true) {
		this->run_ = false;
	}
}

bool RNetService::IsRunning(void) {
	return this->run_;
}

void RNetService::Join(void) {
	this->thread_.join();
}

}


#endif
