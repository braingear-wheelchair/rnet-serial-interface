#ifndef RNETCOUNTER_HPP
#define RNETCOUNTER_HPP

#include <stdint.h>

namespace rnet {

class RNetCounter {
	public:
		static RNetCounter& Instance(void);
		void Increment(void);
		void Decrement(void);
		uint8_t Get(void);
		void Set(uint8_t value);
		void Reset(void);

	private:
		RNetCounter(void);
  		~RNetCounter(void)= default;
  		RNetCounter(const RNetCounter&)= delete;
  		RNetCounter& operator = (const RNetCounter&) = delete;

	private:
		uint8_t counter_;
};


}



#endif
