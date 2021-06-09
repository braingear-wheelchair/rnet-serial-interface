#ifndef RNETSERVICE_HPP
#define RNETSERVICE_HPP

#include <chrono>
#include <thread>

namespace rnetserial {

class RNetService {
	
	public:
		RNetService(void);
		~RNetService(void);

		virtual void Run(void) = 0;
		void Start(void);
		void Stop(void);
		void Join(void);
		bool IsRunning(void);

	private:
		bool run_;
		std::thread thread_;


};


}

#endif
