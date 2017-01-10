#include <randnum/generator.hpp>
#include <boost/test/unit_test.hpp>

namespace test {
	namespace randnum {
		BOOST_AUTO_TEST_SUITE(generator)

		BOOST_AUTO_TEST_CASE(basics) {
			::randnum::generator object(1);
			
			BOOST_REQUIRE(object.seed() == 1);
			BOOST_REQUIRE(object.get_position() == 0);
		}

		BOOST_AUTO_TEST_CASE(position) {
			::randnum::generator object(1);
			object.discard(4);
			
			BOOST_REQUIRE(object.get_position() == 4);
			object.discard(0);
			BOOST_REQUIRE(object.get_position() == 4);
		}

		BOOST_AUTO_TEST_CASE(seed) {
			::randnum::generator object1;
			::randnum::generator object2;
			
			BOOST_REQUIRE(object1.seed() != object2.seed());
		}

		BOOST_AUTO_TEST_CASE(deterministic) {
			BOOST_CHECK(typeid(::randnum::generator::generator_type) == typeid(::boost::random::mt11213b));
			
			if(typeid(::randnum::generator::generator_type) == typeid(::boost::random::mt11213b)) {
				::randnum::generator object(1);
				
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 1);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 1);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 1);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 0);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 0);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 1);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 1);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 0);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 1);
				BOOST_REQUIRE(object.get_uniform_integer(0, 1) == 1);
			}
		}

		BOOST_AUTO_TEST_CASE(uniform_integer) {
			::randnum::generator object;
			
			std::size_t length = 50000;
			std::uint_fast64_t sum = 0;
			double mean = 0;
			
			for(std::size_t i = 0; i < length; i++) {
				sum += object.get_uniform_integer(0, 10);
			}
			
			mean = sum/(double)length;
			
			BOOST_CHECK(mean >= 4.9 && mean <= 5.1);
		}

		BOOST_AUTO_TEST_CASE(uniform_real) {
			::randnum::generator object;
			
			std::size_t length = 50000;
			double sum = 0;
			double mean = 0;
			
			for(std::size_t i = 0; i < length; i++) {
				sum += object.get_uniform_real(0.0, 1.0);
			}
			
			mean = sum/(double)length;
			
			BOOST_CHECK(mean >= 0.49 && mean <= 0.51);
		}

		BOOST_AUTO_TEST_CASE(weighted_integer) {
			::randnum::generator object;
			
			std::size_t length = 50000;
			double sum = 0;
			double mean = 0;
			double weights[2] = {0.25, 0.75};
			
			for(std::size_t i = 0; i < length; i++) {
				sum += object.get_weighted_integer(weights, sizeof(weights)/sizeof(weights[0]));
			}
			
			mean = sum/(double)length;
			
			BOOST_CHECK(mean >= 0.74 && mean <= 0.76);
		}

		BOOST_AUTO_TEST_SUITE_END()
	}
}
