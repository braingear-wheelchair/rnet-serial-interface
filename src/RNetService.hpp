#ifndef RNETSERVICE_HPP
#define RNETSERVICE_HPP

#include <chrono>
#include <mutex>
#include <thread>

namespace rnet {

class RNetService {
	
	public:
		RNetService(void);
		~RNetService(void);

		virtual void Run(void) = 0;
		void Start(void);
		void Stop(void);
		void Join(void);
		bool IsRunning(void);
		std::string name(void);

	protected:
		std::string name_;
	private:
		bool run_;
		std::thread thread_;
		std::mutex mutex_;


};


}

#endif
