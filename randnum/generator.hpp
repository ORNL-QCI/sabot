#ifndef _RANDNUM_GENERATOR_HPP
#define _RANDNUM_GENERATOR_HPP

#include <common.hpp>
#if !SABOT_NTHRD_RNG
#	include <mutex>
#endif
#include <boost/random.hpp>

namespace randnum {
	/**
	 * \brief The types of generators supported.
	 * 
	 * We can use any random generator implemented in the boost library. If you are using
	 * an older version of the library, some of these may not be available. In this case,
	 * the compiler will throw a compile-time error.
	 */
	namespace gen_type {
		using minstd_rand0 = boost::random::minstd_rand0;
		using minstd_rand = boost::random::minstd_rand;
		using rand48 = boost::random::rand48;
		using ecuyer1988 = boost::random::ecuyer1988;
		using knuth_b = boost::random::knuth_b;
		using krutzer1986 = boost::random::kreutzer1986;
		using taus88 = boost::random::taus88;
		using hellekalek1995 = boost::random::hellekalek1995;
		using mt11213b = boost::random::mt11213b;
		using mt19937 = boost::random::mt19937;
		using mt19937_64 = boost::random::mt19937_64;
		using lagged_fibonacci607 = boost::random::lagged_fibonacci607;
		using lagged_fibonacci1279 = boost::random::lagged_fibonacci1279;
		using lagged_fibonacci2281 = boost::random::lagged_fibonacci2281;
		using lagged_fibonacci3217 = boost::random::lagged_fibonacci3217;
		using lagged_fibonacci4423 = boost::random::lagged_fibonacci4423;
		using lagged_fibonacci9689 = boost::random::lagged_fibonacci9689;
		using lagged_fibonacci19937 = boost::random::lagged_fibonacci19937;
		using lagged_fibonacci23209 = boost::random::lagged_fibonacci23209;
		using lagged_fibonacci44497 = boost::random::lagged_fibonacci44497;
		using ranlux3 = boost::random::ranlux3;
		using ranlux4 = boost::random::ranlux4;
		using ranlux64_3 = boost::random::ranlux64_3;
		using ranlux64_4 = boost::random::ranlux64_4;
		using ranlux3_01 = boost::random::ranlux3_01;
		using ranlux4_01 = boost::random::ranlux4_01;
		using ranlux64_3_01 = boost::random::ranlux64_3_01;
		using ranlux64_4_01 = boost::random::ranlux64_4_01;
		using ranlux24 = boost::random::ranlux24;
		using ranlux48 = boost::random::ranlux48;
	}
	
	/**
	 * \brief A random number generator.
	 * 
	 * Each function that returns a random value is driven by the same random number
	 * generator.
	 * 
	 * \note Threadsafe if SABOT_NTHRD_RNG is false.
	 */
	class generator {
	 private:
		#if !SABOT_NTHRD_RNG
		/**
		 * \brief Standard lock type for the class.
		 */
		using lock_t = std::lock_guard<std::mutex>;
		#endif
	
	 public:
	 	/**
		 * \todo Finish implementing flexibility in this.
		 */
		using generator_type = gen_type::mt11213b;
		
		/**
		 * \brief Constructor with no supplied seed.
		 * 
		 * We will try our best at picking a nontrivial seed. We look at the system clock
		 * in UTC with microsecond precision as the seed. On Windows machines this
		 * precision is reduced to milliseconds. Discussion of this is found in the Boost
		 * documentation.
		 * 
		 * It is important to note that we may lose precision when using the time.
		 */
		generator();
		
		/**
		 * \brief Constructor takes in a seed value.
		 */
		generator(const uint32_t suppliedSeed);
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		generator(const generator&) = delete;
		
		/**
		 * \brief Move constructor is disabled.
		 */
		generator(generator&&) = delete;
		
		/**
		 * \brief Assignment operator is disabled.
		 */
		generator& operator=(const generator&) = delete;
		
		/**
		 * \brief Move assignment operator is disabled.
		 */
		generator& operator=(generator&&) = delete;
		
		/**
		 * \brief Destructor.
		 */
		~generator() {
		}
		
		/**
		 * \brief Return an integer between two numbers with a uniform distribution.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		template <typename T, T lower, T upper>
				T get_uniform_integer() {
			static_assert(std::is_integral<T>::value,
					"T must be an integral type.");
			static_assert(lower <= upper,
					"lower cannot be larger than upper.");
			
			boost::random::uniform_int_distribution<T> distribution(lower, upper);
			
			#if !SABOT_NTHRD_RNG
			lock_t lock(accessMutex);
			#endif
			
			position++;
			
			return distribution(rng);
		}
		
		/**
		 * \brief Return an integer between two numbers with a uniform distribution.
		 * 
		 * \warning The compiler will try to deduce the template parameter T from the
		 * function arguments if T is not supplied. Therefore it is possible to call the
		 * function without any indication it is a template function. If this is done,
		 * then the arguments must be of a consistent type.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		template <typename T>
				T get_uniform_integer(const T lower = 0, const T upper = 100) {
			static_assert(std::is_integral<T>::value,
					"T must be an integral type.");
			
			#ifdef SABOT_THROW
			if(UNLIKELY(lower > upper)) {
				throw std::invalid_argument(err_msg::_badvals);
			}
			#endif
			
			boost::random::uniform_int_distribution<T> distribution(lower, upper);
			
			#if !SABOT_NTHRD_RNG
			lock_t lock(accessMutex);
			#endif
			
			position++;
			
			return distribution(rng);
		}
		
		/**
		 * \brief Return a real number between two numbers with a uniform distribution.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		template <typename T, T lower, T upper>
				T get_uniform_real() {
			static_assert(std::is_floating_point<T>::value,
					"T must be a floating point type.");
			static_assert(lower <= upper, "lower cannot be larger than upper.");
			
			boost::random::uniform_real_distribution<T> distribution(lower, upper);
			
			#if !SABOT_NTHRD_RNG
			lock_t lock(accessMutex);
			#endif
			
			position++;
			
			return distribution(rng);
		}
		
		/**
		 * \brief Return a real number between two numbers with a uniform distribution.
		 * 
		 * \warning The compiler will try to deduce the template parameter T from the
		 * function arguments if T is not supplied. Therefore it is possible to call the
		 * function without any indication it is a template function. If this is done,
		 * then the arguments must be of a consistent type.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		template <typename T>
				T get_uniform_real(const T lower = 0.0, const T upper = 1.0) {
			static_assert(std::is_floating_point<T>::value,
					"T must be a floating point type.");
			
			#ifdef SABOT_THROW
			if(UNLIKELY(lower > upper)) {
				throw std::invalid_argument(err_msg::_badvals);
			}
			#endif
			
			boost::random::uniform_real_distribution<T> distribution(lower, upper);
			
			#if !SABOT_NTHRD_RNG
			lock_t lock(accessMutex);
			#endif
			
			position++;
			
			return distribution(rng);
		}
		
		/**
		 * \brief Return an integer number between 0 and the number of elements in the
		 * supplied weight list, where each value in the list corresponds to the numbers
		 * weight.
		 * 
		 * \warning The compiler will try to deduce the template parameter T from the
		 * function arguments if T is not supplied. Therefore it is possible to call the
		 * function without any indication it is a template function. If this is done,
		 * then the arguments must be of a consistent type.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		template <typename T>
				std::size_t get_weighted_integer(const T* const weights,
				const SABOT_UINTN size) {
			#ifdef SABOT_THROW
			if(UNLIKELY(weights == 0)) {
				throw std::invalid_argument(err_msg::_nllpntr);
			}
			if(UNLIKELY(size == 0)) {
				throw std::invalid_argument(err_msg::_zrlngth);
			}
			#endif
			
			T sample(get_uniform_real((T)0, std::accumulate(weights, weights+size, (T)0)));
			
			for(SABOT_UINTN i = 0; i < size; i++) {
				if(sample < weights[i]) {
					return i;
				}
				sample -= weights[i];
			}
			
			throw std::logic_error(err_msg::_unrchcd);
		}
		
		/**
		 * \brief Return the current seed.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		inline uint32_t seed() const {
			return rngSeed;
		}
		
		/**
		 * \brief Return the current position of the random number generator source.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		inline SABOT_UINTN get_position() {
			#if !SABOT_NTHRD_RNG
			lock_t lock(accessMutex);
			#endif
			
			return position;
		}
		
		/**
		 * \brief Discard n samples.
		 * 
		 * \note Threadsafe if SABOT_NTHRD_RNG is false.
		 */
		inline void discard(const SABOT_UINTN n) {
			#if !SABOT_NTHRD_RNG
			lock_t lock(accessMutex);
			#endif
			
			rng.discard(n);
			position += n;
		}
	
	 private:
		/**
		 * \brief The seed of the generator.
		 */
		SABOT_UINTN rngSeed;
		
		/**
		 * \brief The current position of the generator.
		 */
		SABOT_UINTN position;
		
		#if !SABOT_NTHRD_RNG
		/**
		 * \brief Mutex for interacting with the generator.
		 */
		std::mutex accessMutex;
		#endif
		
		/**
		 * \brief The RNG generator.
		 */
		generator_type rng;
	};
}

#endif
