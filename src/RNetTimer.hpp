#ifndef RNET_TIMER_HPP
#define RNET_TIMER_HPP

#include <chrono>

namespace rnet {

using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

template<typename _Clock, typename _Dur>
using time_point = std::chrono::time_point<_Clock, _Dur>;

using default_clock_t = std::conditional< std::chrono::high_resolution_clock::is_steady,
                                          std::chrono::high_resolution_clock,
                                          std::chrono::steady_clock >::type;

/**
 * @brief Timer. A tic-toc timer.
 *
 * Mesure the elapsed time between construction - or tic() -
 * and toc(). The elapsed time is expressed in unit.
 *
 * @param unit. The time unit.
 * @see unit
 */
template <typename unit>
class RNetTimer {
	public:
	
		/**
  		 * @brief Timer. Launch the timer.
  		 */
  		RNetTimer(): start_(now()) { }

  		/**
  		 * @brief ~Timer. Default desctructor.
  		 */
  		~RNetTimer() = default;

  		/**
  		 * @brief tic. Reset the timer.
  		 */
  		void tic() {
			start_ = now();
  		}

  		/**
  		 * @brief toc. Return this elapsed time since construction or last tic().
  		 * @return double. The elapsed time.
  		 * @see tic()
  		 */
  		template <typename T = int64_t>
  		T toc() {
			return static_cast<T>(cast_d(now() - start_).count());
  		}

	protected:

		time_point<default_clock_t, unit> start_;

  		template <typename... Args>
  		auto cast_d(Args&&... args) ->
			decltype(std::chrono::duration_cast<unit>(std::forward<Args>(args)...)) {
			return std::chrono::duration_cast<unit>(std::forward<Args>(args)...);
  		}

  		template <typename... Args>
  		auto cast(Args&&... args) ->
			decltype(std::chrono::time_point_cast<unit>(std::forward<Args>(args)...)) {
			return std::chrono::time_point_cast<unit>(std::forward<Args>(args)...);
  		}

  		auto now() ->
  		decltype(std::declval<RNetTimer<unit>>().cast(default_clock_t::now()))
  		{
  		  return cast(default_clock_t::now());
  		}
};

using timer_secs  = RNetTimer<seconds>;
using timer_msecs = RNetTimer<milliseconds>;
using timer_usecs = RNetTimer<microseconds>;
using timer_nsecs = RNetTimer<nanoseconds>;

}
#endif


