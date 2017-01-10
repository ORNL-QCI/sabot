#include <string_buffer.hpp>
#include <boost/test/unit_test.hpp>

namespace test {
	BOOST_AUTO_TEST_SUITE(string_buffer)
	
	BOOST_AUTO_TEST_CASE(constructors) {
		// Empty constructor with default initial capacity
		::string_buffer<> object1;
		BOOST_REQUIRE(object1.size() == 0);
		BOOST_REQUIRE(object1.capacity() > 0);
		
		// Empty constructor with specified initial capacity
		::string_buffer<5> object2;
		BOOST_REQUIRE(object2.size() == 0);
		BOOST_REQUIRE(object2.capacity() >= 5);
		
		// String constructor with default initial capacity
		const char str[12] = "hello world";
		::string_buffer<> object3(str);
		BOOST_REQUIRE(object3.size() == sizeof(str));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(strcmp(object3.data(), str) == 0);
		
		// String constructor with specified initial capacity
		::string_buffer<31> object4(str);
		BOOST_REQUIRE(object4.size() == sizeof(str));
		BOOST_REQUIRE(object4.capacity() >= 31);
		BOOST_REQUIRE(strcmp(object4.data(), str) == 0);
		
		// String constructor with poorly chosen initial capacity
		::string_buffer<4> object5(str);
		BOOST_REQUIRE(object5.size() == sizeof(str));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(strcmp(object5.data(), str) == 0);
		
		// Memory block constructor with default initial capacity
		const char memoryBlock[11] = {'h', 'e', 'l', 'l', 'o', ' ',
				'w', 'o', 'r', 'l', 'd'};
		::string_buffer<> object6(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(memcmp(object6.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Memory block constructor with specified initial capacity;
		::string_buffer<33> object7(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object7.capacity() >= 33);
		BOOST_REQUIRE(memcmp(object7.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Memory block constructor with pporly chosen initial capacity
		::string_buffer<1> object8(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(memcmp(object8.data(), memoryBlock, sizeof(memoryBlock)) == 0);
	}
	
	BOOST_AUTO_TEST_CASE(moves) {
		// Move constructor for empty constructor with default initial capacity
		::string_buffer<> object11;
		::string_buffer<> object11mc(std::move(object11));
		BOOST_REQUIRE(object11mc.size() == 0);
		BOOST_REQUIRE(object11mc.capacity() > 0);
		
		// Move assignment operator for empty constructor with default initial capacity
		::string_buffer<> object12;
		::string_buffer<> object12mao = std::move(object12);
		BOOST_REQUIRE(object12mao.size() == 0);
		BOOST_REQUIRE(object12mao.capacity() > 0);
		
		// Move constructor for empty constructor with specified initial capacity
		::string_buffer<5> object21;
		::string_buffer<5> object21mc(std::move(object21));
		BOOST_REQUIRE(object21mc.size() == 0);
		BOOST_REQUIRE(object21mc.capacity() >= 5);
		
		// Move assignment operator for empty constructor with specified initial capacity
		::string_buffer<9> object22;
		::string_buffer<9> object22mao = std::move(object22);
		BOOST_REQUIRE(object22mao.size() == 0);
		BOOST_REQUIRE(object22mao.capacity() >= 9);
		
		// Move constructor for string constructor with default initial capacity
		const char str[12] = "hello world";
		::string_buffer<> object31(str);
		::string_buffer<> object31mc(std::move(object31));
		BOOST_REQUIRE(object31mc.size() == sizeof(str));
		BOOST_REQUIRE(object31mc.capacity() >= object31mc.size());
		BOOST_REQUIRE(strcmp(object31mc.data(), str) == 0);
		
		// Move assignment operator for string constructor with default initial capacity
		::string_buffer<> object32(str);
		::string_buffer<> object32mao = std::move(object32);
		BOOST_REQUIRE(object32mao.size() == sizeof(str));
		BOOST_REQUIRE(object32mao.capacity() >= object32mao.size());
		BOOST_REQUIRE(strcmp(object32mao.data(), str) == 0);
		
		// Move constructor for string constructor with specified initial capacity
		::string_buffer<31> object41(str);
		::string_buffer<31> object41mc(std::move(object41));
		BOOST_REQUIRE(object41mc.size() == sizeof(str));
		BOOST_REQUIRE(object41mc.capacity() >= 31);
		BOOST_REQUIRE(strcmp(object41mc.data(), str) == 0);
		
		// Move assignment operator for string constructor with specified initial capacity
		::string_buffer<37> object42(str);
		::string_buffer<37> object42mao = std::move(object42);
		BOOST_REQUIRE(object42mao.size() == sizeof(str));
		BOOST_REQUIRE(object42mao.capacity() >= 37);
		BOOST_REQUIRE(strcmp(object42mao.data(), str) == 0);
		
		// Move constructor for string constructor with poorly chosen initial capacity
		::string_buffer<1> object51(str);
		::string_buffer<1> object51mc(std::move(object51));
		BOOST_REQUIRE(object51mc.size() == sizeof(str));
		BOOST_REQUIRE(object51mc.capacity() >= object51mc.size());
		BOOST_REQUIRE(strcmp(object51mc.data(), str) == 0);
		
		// Move assignment operator for string constructor with poorly chosen initial
		// capacity
		::string_buffer<7> object52(str);
		::string_buffer<7> object52mao = std::move(object52);
		BOOST_REQUIRE(object52mao.size() == sizeof(str));
		BOOST_REQUIRE(object52mao.capacity() >= object52mao.size());
		BOOST_REQUIRE(strcmp(object52mao.data(), "hello world") == 0);
		
		// Move constructor for memory block constructor with default initial capacity
		const char memoryBlock[11] = {'h', 'e', 'l', 'l', 'o', ' ',
				'w', 'o', 'r', 'l', 'd'};
		::string_buffer<> object61(memoryBlock, sizeof(memoryBlock));
		::string_buffer<> object61mc(std::move(object61));
		BOOST_REQUIRE(object61mc.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object61mc.capacity() >= object61mc.size());
		BOOST_REQUIRE(memcmp(object61mc.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Move assignment operator for memory block constructor with default initial
		// capacity
		::string_buffer<> object62(memoryBlock, sizeof(memoryBlock));
		::string_buffer<> object62mao = std::move(object62);
		BOOST_REQUIRE(object62mao.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object62mao.capacity() >= object62mao.size());
		BOOST_REQUIRE(memcmp(object62mao.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Move constructor for memory block constructor with specified initial capacity
		::string_buffer<35> object71(memoryBlock, sizeof(memoryBlock));
		::string_buffer<35> object71mc(std::move(object71));
		BOOST_REQUIRE(object71mc.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object71mc.capacity() >= 35);
		BOOST_REQUIRE(memcmp(object71mc.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Move assignment operator for memory block constructor with specified initial
		// capacity
		::string_buffer<51> object72(memoryBlock, sizeof(memoryBlock));
		::string_buffer<51> object72mao = std::move(object72);
		BOOST_REQUIRE(object72mao.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object72mao.capacity() >= 51);
		BOOST_REQUIRE(memcmp(object72mao.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Move constructor for memory block constructor with poorly chosen initial
		// capacity
		::string_buffer<3> object81(memoryBlock, sizeof(memoryBlock));
		::string_buffer<3> object81mc(std::move(object81));
		BOOST_REQUIRE(object81mc.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object81mc.capacity() >= object81mc.size());
		BOOST_REQUIRE(memcmp(object81mc.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Move assignment operator for memory block constructor with poorly chosen
		// initial capacity
		::string_buffer<5> object82(memoryBlock, sizeof(memoryBlock));
		::string_buffer<5> object82mao = std::move(object82);
		BOOST_REQUIRE(object82mao.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object82mao.capacity() >= object82mao.size());
		BOOST_REQUIRE(memcmp(object82mao.data(), memoryBlock, sizeof(memoryBlock)) == 0);
	}
	
	BOOST_AUTO_TEST_CASE(concatenation_str_small) {
		// Empty constructor with default capacity
		const char str[12] = "hello world";
		::string_buffer<> object1;
		BOOST_REQUIRE(object1.size() == 0);
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		object1 += str;
		BOOST_REQUIRE(object1.size() == sizeof(str));
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		BOOST_REQUIRE(strcmp(object1.data(), str) == 0);
		
		// Empty constructor with specified capacity
		::string_buffer<43> object2;
		BOOST_REQUIRE(object2.size() == 0);
		BOOST_REQUIRE(object2.capacity() >= 43);
		object2 += str;
		BOOST_REQUIRE(object2.size() == sizeof(str));
		BOOST_REQUIRE(object2.capacity() >= 43);
		BOOST_REQUIRE(strcmp(object2.data(), str) == 0);
		
		// String constructor with default capacity
		const char strFirstHalf[7] = "hello ";
		const char strSecondHalf[6] = "world";
		char strCombined[sizeof(strFirstHalf)+sizeof(strSecondHalf)-1];
		strcpy(strCombined, strFirstHalf);
		strcat(strCombined, strSecondHalf);
		::string_buffer<> object3(strFirstHalf);
		BOOST_REQUIRE(object3.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(strcmp(object3.data(), strFirstHalf) == 0);
		object3 += strSecondHalf;
		BOOST_REQUIRE(object3.size() == sizeof(strCombined));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(strcmp(object3.data(), strCombined) == 0);
		
		// String constructor with specified capacity
		::string_buffer<25> object4(strFirstHalf);
		BOOST_REQUIRE(object4.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(strcmp(object4.data(), strFirstHalf) == 0);
		object4 += strSecondHalf;
		BOOST_REQUIRE(object4.size() == sizeof(strCombined));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(strcmp(object4.data(), strCombined) == 0);
		
		// String constructor with poorly chosen capacity
		::string_buffer<1> object5(strFirstHalf);
		BOOST_REQUIRE(object5.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(strcmp(object5.data(), strFirstHalf) == 0);
		object5 += strSecondHalf;
		BOOST_REQUIRE(object5.size() == sizeof(strCombined));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(strcmp(object5.data(), strCombined) == 0);
		
		// Memory block constructor with default capacity
		const char memoryBlock[6] = {'h','e','l', 'l', 'o', ' '};
		char memoryBlockCombined[sizeof(memoryBlock)+sizeof(strSecondHalf)] = {'\0'};
		memcpy(memoryBlockCombined, memoryBlock, sizeof(memoryBlock));
		strcpy(&memoryBlockCombined[sizeof(memoryBlock)], strSecondHalf);
		::string_buffer<> object6(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(memcmp(object6.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		object6 += strSecondHalf;
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlockCombined));
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(strcmp(object6.data(), memoryBlockCombined) == 0);
		
		// Memory block constructor with specified capacity
		::string_buffer<23> object7(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object7.capacity() >= 23);
		object7 += strSecondHalf;
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlockCombined));
		BOOST_REQUIRE(object7.capacity() >= 23);
		BOOST_REQUIRE(strcmp(object7.data(), memoryBlockCombined) == 0);
		
		// Memory block constructor with poorly chosen capacity
		::string_buffer<3> object8(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		object8 += strSecondHalf;
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlockCombined));
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(strcmp(object8.data(), memoryBlockCombined) == 0);
	}
	
	BOOST_AUTO_TEST_CASE(concatenation_str_big) {
		const char str[45] = "hello worldhello worldhello worldhello world";
		// Empty constructor with default capacity
		::string_buffer<> object1;
		BOOST_REQUIRE(object1.size() == 0);
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		object1 += str;
		BOOST_REQUIRE(object1.size() == sizeof(str));
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		BOOST_REQUIRE(strcmp(object1.data(), str) == 0);
		
		// Empty constructor with specified capacity
		::string_buffer<2> object2;
		BOOST_REQUIRE(object2.size() == 0);
		BOOST_REQUIRE(object2.capacity() >= 2);
		object2 += str;
		BOOST_REQUIRE(object2.size() == sizeof(str));
		BOOST_REQUIRE(object2.capacity() >= 2);
		BOOST_REQUIRE(strcmp(object2.data(), str) == 0);
		
		// String constructor with default capacity
		const char strFirstHalf[22] = "hello worldhelloworld";
		const char strSecondHalf[22] = "hello worldhelloworld";
		char strCombined[sizeof(strFirstHalf)+sizeof(strSecondHalf)-1] = {'\0'};
		strcpy(strCombined, strFirstHalf);
		strcat(strCombined, strSecondHalf);
		::string_buffer<> object3(strFirstHalf);
		BOOST_REQUIRE(object3.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(strcmp(object3.data(), strFirstHalf) == 0);
		object3 += strSecondHalf;
		BOOST_REQUIRE(object3.size() == sizeof(strCombined));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(strcmp(object3.data(), strCombined) == 0);
		
		// String constructor with specified capacity
		::string_buffer<25> object4(strFirstHalf);
		BOOST_REQUIRE(object4.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(strcmp(object4.data(), strFirstHalf) == 0);
		object4 += strSecondHalf;
		BOOST_REQUIRE(object4.size() == sizeof(strCombined));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(strcmp(object4.data(), strCombined) == 0);
		
		// String constructor with poorly chosen capacity
		::string_buffer<1> object5(strFirstHalf);
		BOOST_REQUIRE(object5.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(strcmp(object5.data(), strFirstHalf) == 0);
		object5 += strSecondHalf;
		BOOST_REQUIRE(object5.size() == sizeof(strCombined));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(strcmp(object5.data(), strCombined) == 0);
		
		// Memory block constructor with default capacity
		const char memoryBlock[21] = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd',
				'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd', '\0'};
		::string_buffer<> object6(memoryBlock, sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(memcmp(object6.data(), memoryBlock, sizeof(memoryBlock)/2) == 0);
		object6 += &memoryBlock[sizeof(memoryBlock)/2];
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(strcmp(object6.data(), memoryBlock) == 0);
		
		// Memory block constructor with specified capacity
		::string_buffer<24> object7(memoryBlock, sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object7.capacity() >= 24);
		BOOST_REQUIRE(memcmp(object7.data(), memoryBlock, sizeof(memoryBlock)/2) == 0);
		object7 += &memoryBlock[sizeof(memoryBlock)/2];
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object7.capacity() >= 24);
		BOOST_REQUIRE(strcmp(object7.data(), memoryBlock) == 0);
		
		// Memory block constructor with poorly chosen capacity
		::string_buffer<3> object8(memoryBlock, sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(memcmp(object8.data(), memoryBlock, sizeof(memoryBlock)/2) == 0);
		object8 += &memoryBlock[sizeof(memoryBlock)/2];
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(strcmp(object8.data(), memoryBlock) == 0);
	}
	
	BOOST_AUTO_TEST_CASE(concatenation_data_small) {
		// Empty constructor with default capacity
		const char str[11] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
		::string_buffer<> object1;
		BOOST_REQUIRE(object1.size() == 0);
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		object1.add(str, sizeof(str));
		BOOST_REQUIRE(object1.size() == sizeof(str));
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		BOOST_REQUIRE(memcmp(object1.data(), str, sizeof(str)) == 0);
		
		// Empty constructor with specified capacity
		::string_buffer<43> object2;
		BOOST_REQUIRE(object2.size() == 0);
		BOOST_REQUIRE(object2.capacity() >= 43);
		object2.add(str, sizeof(str));
		BOOST_REQUIRE(object2.size() == sizeof(str));
		BOOST_REQUIRE(object2.capacity() >= 43);
		BOOST_REQUIRE(memcmp(object2.data(), str, sizeof(str)) == 0);
		
		// String constructor with default capacity
		const char strFirstHalf[7] = {'h', 'e', 'l', 'l', 'o', ' ', '\0'};
		const char strSecondHalf[6] = {'w', 'o', 'r', 'l', 'd'};
		char strCombined[sizeof(strFirstHalf)+sizeof(strSecondHalf)];
		memcpy(strCombined, strFirstHalf, sizeof(strFirstHalf));
		memcpy(&strCombined[sizeof(strFirstHalf)],
				strSecondHalf,
				sizeof(strSecondHalf));
		::string_buffer<> object3(strFirstHalf);
		BOOST_REQUIRE(object3.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(memcmp(object3.data(), strFirstHalf, sizeof(strFirstHalf)) == 0);
		object3.add(strSecondHalf, sizeof(strSecondHalf));
		BOOST_REQUIRE(object3.size() == sizeof(strCombined));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(memcmp(object3.data(), strCombined, sizeof(strCombined)) == 0);
		
		// String constructor with specified capacity
		::string_buffer<25> object4(strFirstHalf);
		BOOST_REQUIRE(object4.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(memcmp(object4.data(), strFirstHalf, sizeof(strFirstHalf)) == 0);
		object4.add(strSecondHalf, sizeof(strSecondHalf));
		BOOST_REQUIRE(object4.size() == sizeof(strCombined));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(memcmp(object4.data(), strCombined, sizeof(strCombined)) == 0);
		
		// String constructor with poorly chosen capacity
		::string_buffer<1> object5(strFirstHalf);
		BOOST_REQUIRE(object5.size() == sizeof(strFirstHalf));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(memcmp(object5.data(), strFirstHalf, sizeof(strFirstHalf)) == 0);
		object5.add(strSecondHalf, sizeof(strSecondHalf));
		BOOST_REQUIRE(object5.size() == sizeof(strCombined));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(memcmp(object5.data(), strCombined, sizeof(strCombined)) == 0);
		
		// Memory block constructor with default capacity
		const char memoryBlock[6] = {'h','e','l', 'l', 'o', ' '};
		char memoryBlockCombined[sizeof(memoryBlock)+sizeof(strSecondHalf)] = {'\0'};
		memcpy(memoryBlockCombined, memoryBlock, sizeof(memoryBlock));
		memcpy(&memoryBlockCombined[sizeof(memoryBlock)],
				strSecondHalf,
				sizeof(strSecondHalf));
		::string_buffer<> object6(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(memcmp(object6.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		object6.add(strSecondHalf, sizeof(strSecondHalf));
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlockCombined));
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(memcmp(object6.data(),
				memoryBlockCombined,
				sizeof(memoryBlockCombined)) == 0);
		
		// Memory block constructor with specified capacity
		::string_buffer<23> object7(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object7.capacity() >= 23);
		BOOST_REQUIRE(memcmp(object7.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		object7.add(strSecondHalf, sizeof(strSecondHalf));
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlockCombined));
		BOOST_REQUIRE(object7.capacity() >= 23);
		BOOST_REQUIRE(memcmp(object7.data(),
				memoryBlockCombined,
				sizeof(memoryBlockCombined)) == 0);
		
		// Memory block constructor with poorly chosen capacity
		::string_buffer<3> object8(memoryBlock, sizeof(memoryBlock));
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(memcmp(object8.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		object8.add(strSecondHalf, sizeof(strSecondHalf));
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlockCombined));
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(memcmp(object8.data(),
				memoryBlockCombined,
				sizeof(memoryBlockCombined)) == 0);
	}
	
	BOOST_AUTO_TEST_CASE(concatenation_data_big) {
		char data[44] = {'\0'};
		const char tempStr[45] = "hello worldhello worldhello worldhello world";
		memcpy(data, "hello worldhello worldhello worldhello world", strlen(tempStr)); 
		// Empty constructor with default capacity
		::string_buffer<> object1;
		BOOST_REQUIRE(object1.size() == 0);
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		object1.add(data, sizeof(data));
		BOOST_REQUIRE(object1.size() == sizeof(data));
		BOOST_REQUIRE(object1.capacity() >= object1.size());
		BOOST_REQUIRE(memcmp(object1.data(), data, sizeof(data)) == 0);
		
		// Empty constructor with specified capacity
		::string_buffer<2> object2;
		BOOST_REQUIRE(object2.size() == 0);
		BOOST_REQUIRE(object2.capacity() >= 2);
		object2.add(data, sizeof(data));
		BOOST_REQUIRE(object2.size() == sizeof(data));
		BOOST_REQUIRE(object2.capacity() >= 2);
		BOOST_REQUIRE(memcmp(object2.data(), data, sizeof(data)) == 0);
		
		// String constructor with default capacity
		char dataFirstHalf[22] = {'\0'};
		const char strTempFirstHalf[22] = "hello worldhelloworld";
		memcpy(dataFirstHalf, strTempFirstHalf, sizeof(strTempFirstHalf));
		char dataSecondHalf[21] = {'\0'};
		const char strTempSecondHalf[22] = "hello worldhelloworld";
		memcpy(dataSecondHalf, strTempSecondHalf, strlen(strTempSecondHalf));
		char dataCombined[sizeof(dataFirstHalf)+sizeof(dataSecondHalf)] = {'\0'};
		memcpy(dataCombined, strTempFirstHalf, sizeof(dataFirstHalf));
		memcpy(&dataCombined[sizeof(dataFirstHalf)],
				strTempSecondHalf, 
				strlen(strTempSecondHalf));
		::string_buffer<> object3(dataFirstHalf);
		BOOST_REQUIRE(object3.size() == sizeof(dataFirstHalf));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(memcmp(object3.data(), dataFirstHalf, sizeof(dataFirstHalf)) == 0);
		object3.add(dataSecondHalf, sizeof(dataSecondHalf));
		BOOST_REQUIRE(object3.size() == sizeof(dataCombined));
		BOOST_REQUIRE(object3.capacity() >= object3.size());
		BOOST_REQUIRE(memcmp(object3.data(), dataCombined, sizeof(dataCombined)) == 0);
		
		// String constructor with specified capacity
		::string_buffer<25> object4(dataFirstHalf);
		BOOST_REQUIRE(object4.size() == sizeof(dataFirstHalf));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(memcmp(object4.data(), dataFirstHalf, sizeof(dataFirstHalf)) == 0);
		object4.add(dataSecondHalf, sizeof(dataSecondHalf));
		BOOST_REQUIRE(object4.size() == sizeof(dataCombined));
		BOOST_REQUIRE(object4.capacity() >= 25);
		BOOST_REQUIRE(memcmp(object4.data(), dataCombined, sizeof(dataCombined)) == 0);
		
		// String constructor with poorly chosen capacity
		::string_buffer<1> object5(dataFirstHalf);
		BOOST_REQUIRE(object5.size() == sizeof(dataFirstHalf));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(memcmp(object5.data(), dataFirstHalf, sizeof(dataFirstHalf)) == 0);
		object5.add(dataSecondHalf, sizeof(dataSecondHalf));
		BOOST_REQUIRE(object5.size() == sizeof(dataCombined));
		BOOST_REQUIRE(object5.capacity() >= object5.size());
		BOOST_REQUIRE(memcmp(object5.data(), dataCombined, sizeof(dataCombined)) == 0);
		
		// Memory block constructor with default capacity
		const char memoryBlock[20] = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd',
				'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};
		::string_buffer<> object6(memoryBlock, sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(memcmp(object6.data(), memoryBlock, sizeof(memoryBlock)/2) == 0);
		object6.add(&memoryBlock[sizeof(memoryBlock)/2],
				sizeof(memoryBlock) - sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object6.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object6.capacity() >= object6.size());
		BOOST_REQUIRE(memcmp(object6.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Memory block constructor with specified capacity
		::string_buffer<24> object7(memoryBlock, sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object7.capacity() >= 24);
		BOOST_REQUIRE(memcmp(object7.data(), memoryBlock, sizeof(memoryBlock)/2) == 0);
		object7.add(&memoryBlock[sizeof(memoryBlock)/2],
				sizeof(memoryBlock) - sizeof(memoryBlock)/2);;
		BOOST_REQUIRE(object7.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object7.capacity() >= 24);
		BOOST_REQUIRE(memcmp(object7.data(), memoryBlock, sizeof(memoryBlock)) == 0);
		
		// Memory block constructor with poorly chosen capacity
		::string_buffer<3> object8(memoryBlock, sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(memcmp(object8.data(), memoryBlock, sizeof(memoryBlock)/2) == 0);
		object8.add(&memoryBlock[sizeof(memoryBlock)/2],
				sizeof(memoryBlock) - sizeof(memoryBlock)/2);
		BOOST_REQUIRE(object8.size() == sizeof(memoryBlock));
		BOOST_REQUIRE(object8.capacity() >= object8.size());
		BOOST_REQUIRE(memcmp(object8.data(), memoryBlock, sizeof(memoryBlock)) == 0);
	}
	
	BOOST_AUTO_TEST_SUITE_END()
}
