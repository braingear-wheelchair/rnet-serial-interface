#include <chrono>
#include <iostream>
#include "RNetService.hpp"

namespace rnet {

class ServiceA: public RNetService {

	public:
		ServiceA(void) {
			this->first_ = true;
		}

		~ServiceA(void) {
		}

		void Run(void) {
		
			while(this->IsRunning()) {
				if(this->first_ == true) {
					printf("SeviceA running\n");
					this->first_ = false;
				}



				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			this->first_ = false;
			printf("SeviceA NOT running\n");

		}

	private:
		bool first_;
};
}

int main(int argc, char** argv) {


	rnet::ServiceA SrvA;
	rnet::ServiceA SrvB;


	SrvA.Start();
	SrvB.Start();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	SrvA.Stop();
	SrvB.Stop();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	return 0;
}
