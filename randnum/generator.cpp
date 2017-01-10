#include "generator.hpp"
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace randnum {
	generator::generator()
			: position(0) {
		using namespace boost::posix_time;
		
		// Using assignment operator for the epoch results in conversion error
		const ptime epoch(boost::gregorian::date(1970, 1, 1));
		const auto timeOfDay = boost::posix_time::microsec_clock::universal_time();
		const auto td = boost::posix_time::time_duration(epoch - timeOfDay);
		rngSeed = td.total_microseconds();
		
		rng.seed(rngSeed);
	}
	
	generator::generator(const uint32_t suppliedSeed)
			: rngSeed(suppliedSeed),
			position(0) {
		rng.seed(rngSeed);
	}
}
