#include <ms_container.hpp>
#include <boost/test/unit_test.hpp>

namespace test {
	BOOST_AUTO_TEST_SUITE(ms_container)
	
	struct fixture {
	 public:
		fixture(const bool option, const char* const str)
				: _option(option) {
			_str = new char[strlen(str)+1];
			strcpy(_str, str);
		}
		
		fixture(const fixture&) = delete;
		
		fixture(fixture&& old) {
			_option = old._option;
			_str = old._str;
			old._str = 0;
		}
			
		fixture& operator=(const fixture&) = delete;
		
		fixture& operator=(fixture&& old) {
			_option = old._option;
			_str = old._str;
			old._str = 0;
			
			return *this;
		}
		
		~fixture() {
			if(_str != 0) {
				delete[] _str;
			}
		}
		
		bool option() const {
			return _option;
		}
		
		const char* str() const {
			return _str;
		}
		
		void set_option(const bool option) {
			_option = option;
		}
		
		void set_str(const char* const str) {
			if(_str != 0) {
				delete[] _str;
			}
			_str = new char[strlen(str)+1];
			strcpy(_str, str);
		}
	
	 private:
		bool _option;
		char* _str;
	};

	BOOST_AUTO_TEST_CASE(numericitemnolabel) {
		ms_container_item<std::size_t, unsigned int> item(1, 5542);
		
		BOOST_REQUIRE(item.id() == 1);
		BOOST_REQUIRE(item.data() == 5542);
		BOOST_REQUIRE(strlen(item.label()) == 0);
		
		auto item2(std::move(item));
		BOOST_REQUIRE(item2.id() == 1);
		BOOST_REQUIRE(item2.data() == 5542);
		BOOST_REQUIRE(strlen(item2.label()) == 0);
		
		auto item3 = std::move(item2);
		BOOST_REQUIRE(item3.id() == 1);
		BOOST_REQUIRE(item3.data() == 5542);
		BOOST_REQUIRE(strlen(item3.label()) == 0);
	}
	
	BOOST_AUTO_TEST_CASE(numericitemlabel) {
		ms_container_item<std::size_t, unsigned int> item(1, 5542, "hello world");
		
		BOOST_REQUIRE(item.id() == 1);
		BOOST_REQUIRE(item.data() == 5542);
		BOOST_REQUIRE(strcmp(item.label(),  "hello world") == 0);
		
		auto item2(std::move(item));
		BOOST_REQUIRE(item2.id() == 1);
		BOOST_REQUIRE(item2.data() == 5542);
		BOOST_REQUIRE(strcmp(item2.label(),  "hello world") == 0);
		
		auto item3 = std::move(item2);
		BOOST_REQUIRE(item3.id() == 1);
		BOOST_REQUIRE(item3.data() == 5542);
		BOOST_REQUIRE(strcmp(item3.label(),  "hello world") == 0);
	}
	
	BOOST_AUTO_TEST_CASE(stringitemnolabel) {
		ms_container_item<std::size_t, const char*> item(1, "hello world");
		
		BOOST_REQUIRE(item.id() == 1);
		BOOST_REQUIRE(strcmp(item.data(), "hello world") == 0);
		BOOST_REQUIRE(strlen(item.label()) == 0);
		
		auto item2(std::move(item));
		BOOST_REQUIRE(item2.id() == 1);
		BOOST_REQUIRE(strcmp(item2.data(), "hello world") == 0);
		BOOST_REQUIRE(strlen(item2.label()) == 0);
		
		auto item3 = std::move(item2);
		BOOST_REQUIRE(item3.id() == 1);
		BOOST_REQUIRE(strcmp(item3.data(), "hello world") == 0);
		BOOST_REQUIRE(strlen(item3.label()) == 0);
	}
	
	BOOST_AUTO_TEST_CASE(stringitemlabel) {
		ms_container_item<std::size_t, const char*> item(1, "hello world", "hello earth");
		
		BOOST_REQUIRE(item.id() == 1);
		BOOST_REQUIRE(strcmp(item.data(), "hello world") == 0);
		BOOST_REQUIRE(strcmp(item.label(),  "hello earth") == 0);
		
		auto item2(std::move(item));
		BOOST_REQUIRE(item2.id() == 1);
		BOOST_REQUIRE(strcmp(item2.data(), "hello world") == 0);
		BOOST_REQUIRE(strcmp(item2.label(),  "hello earth") == 0);
		
		auto item3 = std::move(item2);
		BOOST_REQUIRE(item3.id() == 1);
		BOOST_REQUIRE(strcmp(item2.data(), "hello world") == 0);
		BOOST_REQUIRE(strcmp(item3.label(),  "hello earth") == 0);
	}
	
	BOOST_AUTO_TEST_CASE(objectitemnolabel) {
		ms_container_item<std::size_t, fixture> item(1, fixture(true, "hello world"));
		
		BOOST_REQUIRE(item.id() == 1);
		BOOST_REQUIRE(item.data().option() == true);
		BOOST_REQUIRE(strcmp(item.data().str(), "hello world") == 0);
		BOOST_REQUIRE(strlen(item.label()) == 0);
		
		auto item2(std::move(item));
		BOOST_REQUIRE(item2.id() == 1);
		BOOST_REQUIRE(item2.data().option() == true);
		BOOST_REQUIRE(strcmp(item2.data().str(), "hello world") == 0);
		
		auto item3 = std::move(item2);
		BOOST_REQUIRE(item3.id() == 1);
		BOOST_REQUIRE(item3.data().option() == true);
		BOOST_REQUIRE(strcmp(item3.data().str(), "hello world") == 0);
	}
	
	BOOST_AUTO_TEST_CASE(objectitemlabel) {
		ms_container_item<std::size_t, fixture> item(1, fixture(false, "hello world"), "hello earth");
		
		BOOST_REQUIRE(item.id() == 1);
		BOOST_REQUIRE(item.data().option() == false);
		BOOST_REQUIRE(strcmp(item.data().str(), "hello world") == 0);
		BOOST_REQUIRE(strcmp(item.label(),  "hello earth") == 0);
		
		auto item2(std::move(item));
		BOOST_REQUIRE(item2.id() == 1);
		BOOST_REQUIRE(item2.data().option() == false);
		BOOST_REQUIRE(strcmp(item2.data().str(), "hello world") == 0);
		BOOST_REQUIRE(strcmp(item2.label(),  "hello earth") == 0);
		
		auto item3 = std::move(item2);
		BOOST_REQUIRE(item3.id() == 1);
		BOOST_REQUIRE(item3.data().option() == false);
		BOOST_REQUIRE(strcmp(item3.data().str(), "hello world") == 0);
		BOOST_REQUIRE(strcmp(item3.label(),  "hello earth") == 0);
	}
	
	BOOST_AUTO_TEST_CASE(numericcontainernolabel) {
		::ms_container<std::size_t, unsigned int, 1> container;
		BOOST_REQUIRE(container.size() == 0);
		BOOST_REQUIRE(container.capacity() == 1);
		
		auto id = container.insert(142412);
		BOOST_REQUIRE(container.size() == 1);
		BOOST_REQUIRE(container.capacity() >= 1);
		
		auto id2 = container.insert(6765423);
		BOOST_REQUIRE(container.size() == 2);
		BOOST_REQUIRE(container.capacity() >= 2);
		BOOST_REQUIRE(id != id2);
		
		std::mutex* elementMutex;
		boost::shared_mutex* editMutex;
		auto item = container.get(id, elementMutex, editMutex);
		BOOST_REQUIRE(item == 142412);
		
		auto item2 = container.get(id2, elementMutex, editMutex);
		BOOST_REQUIRE(item2 == 6765423);
		
		BOOST_REQUIRE(item != item2);
		std::size_t fakeId = 1;
		while(fakeId == id || fakeId == id2) {
			fakeId++;
		}
		BOOST_CHECK_THROW(container.get(fakeId, elementMutex, editMutex), std::out_of_range);
		
		auto item3 = container.insert_and_return_full(543256, elementMutex, editMutex);
		BOOST_REQUIRE(container.size() == 3);
		BOOST_REQUIRE(container.capacity() >= 3);
		BOOST_REQUIRE(item3.second == 543256);
		
		auto eitem = container.edit(id, elementMutex, editMutex);
		BOOST_REQUIRE(eitem = 142412);
		eitem = 345672;
		BOOST_REQUIRE(eitem == 345672);
		
		container.erase(id);
		BOOST_REQUIRE(container.size() ==  2);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_NO_THROW(container.get(id2, elementMutex, editMutex));
		BOOST_CHECK_NO_THROW(container.get(item3.first, elementMutex, editMutex));
		
		container.erase(id2);
		BOOST_REQUIRE(container.size() ==  1);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(id2, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_NO_THROW(container.get(item3.first, elementMutex, editMutex));
		
		container.erase(item3.first);
		BOOST_REQUIRE(container.size() ==  0);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(id2, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(item3.first, elementMutex, editMutex), std::out_of_range);
		
		container.insert_and_return(123432, elementMutex, editMutex);
		BOOST_REQUIRE(container.size() ==  1);
	}
	
	/** \todo */
	//BOOST_AUTO_TEST_CASE(numericcontainerlabel) {
	//}
	
	BOOST_AUTO_TEST_CASE(stringcontainernolabel) {
		::ms_container<std::size_t, const char*, 1> container;
		BOOST_REQUIRE(container.size() == 0);
		BOOST_REQUIRE(container.capacity() == 1);
		
		auto id = container.insert("hello world");
		BOOST_REQUIRE(container.size() == 1);
		BOOST_REQUIRE(container.capacity() >= 1);
		
		auto id2 = container.insert("hello earth");
		BOOST_REQUIRE(container.size() == 2);
		BOOST_REQUIRE(container.capacity() >= 2);
		BOOST_REQUIRE(id != id2);
		
		std::mutex* elementMutex;
		boost::shared_mutex* editMutex;
		auto item = container.get(id, elementMutex, editMutex);
		BOOST_REQUIRE(strcmp(item, "hello world") == 0);
		
		auto item2 = container.get(id2, elementMutex, editMutex);
		BOOST_REQUIRE(strcmp(item2, "hello earth") == 0);
		
		BOOST_REQUIRE(strcmp(item, item2) != 0);
		std::size_t fakeId = 1;
		while(fakeId == id || fakeId == id2) {
			fakeId++;
		}
		BOOST_CHECK_THROW(container.get(fakeId, elementMutex, editMutex), std::out_of_range);
		
		auto item3 = container.insert_and_return_full("hello world", elementMutex, editMutex);
		BOOST_REQUIRE(container.size() == 3);
		BOOST_REQUIRE(container.capacity() >= 3);
		BOOST_REQUIRE(strcmp(item3.second, "hello world") == 0);
		
		auto eitem = container.edit(id, elementMutex, editMutex);
		BOOST_REQUIRE(strcmp(eitem, "hello world") == 0);
		eitem = "hello earth";
		BOOST_REQUIRE(strcmp(eitem, "hello earth") == 0);
		
		container.erase(id);
		BOOST_REQUIRE(container.size() ==  2);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_NO_THROW(container.get(id2, elementMutex, editMutex));
		BOOST_CHECK_NO_THROW(container.get(item3.first, elementMutex, editMutex));
		
		container.erase(id2);
		BOOST_REQUIRE(container.size() ==  1);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(id2, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_NO_THROW(container.get(item3.first, elementMutex, editMutex));
		
		container.erase(item3.first);
		BOOST_REQUIRE(container.size() ==  0);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(id2, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(item3.first, elementMutex, editMutex), std::out_of_range);
		
		container.insert_and_return("hello world", elementMutex, editMutex);
		BOOST_REQUIRE(container.size() ==  1);
	}
	
	/** \todo */
	//BOOST_AUTO_TEST_CASE(stringcontainerlabel) {
	//}
	
	BOOST_AUTO_TEST_CASE(objectcontainernolabel) {
	::ms_container<std::size_t, fixture, 1> container;
		BOOST_REQUIRE(container.size() == 0);
		BOOST_REQUIRE(container.capacity() == 1);
		
		auto id = container.insert(fixture(false, "hello world"));
		BOOST_REQUIRE(container.size() == 1);
		BOOST_REQUIRE(container.capacity() >= 1);
		
		auto id2 = container.insert(fixture(true, "hello earth"));
		BOOST_REQUIRE(container.size() == 2);
		BOOST_REQUIRE(container.capacity() >= 2);
		BOOST_REQUIRE(id != id2);
		
		std::mutex* elementMutex;
		boost::shared_mutex* editMutex;
		auto& item = container.get(id, elementMutex, editMutex);
		BOOST_REQUIRE(strcmp(item.str(), "hello world") == 0);
		BOOST_REQUIRE(item.option() == false);
		
		auto& item2 = container.get(id2, elementMutex, editMutex);
		BOOST_REQUIRE(strcmp(item2.str(), "hello earth") == 0);
		BOOST_REQUIRE(item2.option() == true);
		
		BOOST_REQUIRE(strcmp(item.str(), item2.str()) != 0);
		std::size_t fakeId = 1;
		while(fakeId == id || fakeId == id2) {
			fakeId++;
		}
		BOOST_CHECK_THROW(container.get(fakeId, elementMutex, editMutex), std::out_of_range);
		
		auto item3 = container.insert_and_return_full(fixture(true, "hello world"), elementMutex, editMutex);
		BOOST_REQUIRE(container.size() == 3);
		BOOST_REQUIRE(container.capacity() >= 3);
		BOOST_REQUIRE(strcmp(item3.second.str(), "hello world") == 0);
		BOOST_REQUIRE(item3.second.option() == true);
		
		auto& eitem = container.edit(id, elementMutex, editMutex);
		BOOST_REQUIRE(strcmp(eitem.str(), "hello world") == 0);
		BOOST_REQUIRE(eitem.option() == false);
		eitem.set_str("hello earth");
		eitem.set_option(true);
		BOOST_REQUIRE(strcmp(eitem.str(), "hello earth") == 0);
		BOOST_REQUIRE(eitem.option() == true);
		
		container.erase(id);
		BOOST_REQUIRE(container.size() ==  2);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_NO_THROW(container.get(id2, elementMutex, editMutex));
		BOOST_CHECK_NO_THROW(container.get(item3.first, elementMutex, editMutex));
		
		container.erase(id2);
		BOOST_REQUIRE(container.size() ==  1);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(id2, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_NO_THROW(container.get(item3.first, elementMutex, editMutex));
		
		container.erase(item3.first);
		BOOST_REQUIRE(container.size() ==  0);
		BOOST_CHECK_THROW(container.get(id, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(id2, elementMutex, editMutex), std::out_of_range);
		BOOST_CHECK_THROW(container.get(item3.first, elementMutex, editMutex), std::out_of_range);
		
		container.insert_and_return(fixture(true, "hello"), elementMutex, editMutex);
		BOOST_REQUIRE(container.size() ==  1);
	}
	
	/** \todo */
	//BOOST_AUTO_TEST_CASE(objectcontainerlabel) {
	//}

	BOOST_AUTO_TEST_SUITE_END()
}
