#ifndef RNETBUFFER_HPP
#define RNETBUFFER_HPP

#include <vector>
#include <mutex>
#include "RNetPacket.hpp"

namespace rnetserial {

class RNetBuffer {

	public:
		RNetBuffer(void);
		~RNetBuffer(void);

		void Add(RNetPacket packet);
		void Remove(unsigned int index);
		void Remove(const std::vector<RNetPacket>::iterator it);
		void Clear(void);
		size_t Size(void);

		void Lock(void);
		void Unlock(void);

		std::vector<RNetPacket>::iterator Begin(void);
		std::vector<RNetPacket>::iterator End(void);
		
		std::vector<RNetPacket>::const_iterator Begin(void) const;
		std::vector<RNetPacket>::const_iterator End(void) const;
		
		std::vector<RNetPacket>::const_iterator cBegin(void) const;
		std::vector<RNetPacket>::const_iterator cEnd(void) const;


	private:
		std::vector<RNetPacket> buffer_;
		std::mutex mutex_;


};

}


#endif
