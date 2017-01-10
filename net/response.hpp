#ifndef _NET_RESPONSE_HPP
#define _NET_RESPONSE_HPP

#include <common.hpp>
#include "../string_buffer.hpp"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

/**
 * \brief The string for the result field.
 */
#define INCLUDED_NET_RESPONSE_RESULT "result"

/**
 * \brief The string for the error field.
 */
#define INCLUDED_NET_RESPONSE_ERROR "error"

namespace net {
	/**
	 * \brief A JSON reponse to a client.
	 * 
	 * All responses are UTF-8.
	 */
	struct response {
	 public:
		/**
		 * \brief Construct response with char result.
		 */
		response(const char result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with unsigned char result.
		 */
		response(const unsigned char result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with bool result.
		 */
		response(const bool result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with bool array result.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		response(const bool* const result,
				const std::size_t size,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					rapid_json_array<bool>(result, size, allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with cstring result.
		 * 
		 * \warning This only takes a reference of the string. If you need to copy string
		 * data into the response, use the constructor that accepts the string_buffer.
		 * 
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 */
		response(const char* const result,
				const bool error = false) {
			#ifdef SABOT_THROW
			if(UNLIKELY(result == 0)) {
				throw std::invalid_argument(err_msg::_nllpntr);
			}
			#endif
			
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					::rapidjson::Value().SetString(::rapidjson::StringRef(result)),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an int result.
		 */
		response(const int result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an array of int result.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		response(const int* const result,
				const std::size_t size,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					rapid_json_array<int>(result, size, allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an unsigned int result.
		 */
		response(const unsigned int result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an array of unsigned int result.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		response(const unsigned int* const result,
				const std::size_t size,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					rapid_json_array<unsigned int>(result, size, allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with a long int result.
		 */
		response(const long int result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an array of long int result.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		response(const long int* const result,
				const std::size_t size,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					rapid_json_array<long int>(result, size, allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an unsigned long int result.
		 */
		response(const unsigned long int result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an array of unsigned long int result.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		response(const unsigned long int* const result,
				const std::size_t size,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					rapid_json_array<unsigned long int>(result, size, allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with a float result.
		 */
		response(const float result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an array of float result.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		response(const float* const result,
				const std::size_t size,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					rapid_json_array<float>(result, size, allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with a double result.
		 */
		response(const double result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					result,
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with an array of double result.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		response(const double* const result,
				const std::size_t size,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					rapid_json_array<double>(result, size, allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Construct response with a result_buffer result.
		 */
		response(string_buffer<>&& result,
				const bool error = false) {
			auto dom(_initialize(error));
			auto& allocator(dom.GetAllocator());
			
			dom.AddMember(INCLUDED_NET_RESPONSE_RESULT,
					::rapidjson::Value(result.data(), result.size(), allocator),
					allocator);
			
			_write(dom);
		}
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		response(const response&) = delete;
		
		/**
		 * \brief Move constructor.
		 */
		response(response&& old)
				: _jsonBuffer(std::move(old._jsonBuffer)) {
		}
		
		/**
		 * \brief Assignment operator is disabled.
		 */
		response& operator=(const response&) = delete;
		
		/**
		 * \brief Move assignment operator.
		 */
		response& operator=(response&& old) {
			_jsonBuffer = std::move(old._jsonBuffer);
			
			return *this;
		}
		
		/**
		 * \brief Return a char array that is a JSON encoding of the response.
		 * 
		 * \note This is null-terminated.
		 */
		inline const char* get_json() const {
			return _jsonBuffer.GetString();
		}
		
		/**
		 * \brief Return a the length of the JSON string.
		 * 
		 * \note This does not include the null terminator.
		 */
		inline std::size_t get_json_size() const {
			return _jsonBuffer.GetSize();
		}
	
	 private:
	 	/**
		 * \brief The rapidjson StringBuffer that holds our JSON string.
		 */
		::rapidjson::StringBuffer _jsonBuffer;
		
		/**
		 * \brief Common initialization code invariant of the result type.
		 */
		inline ::rapidjson::Document _initialize(const bool error) {
			::rapidjson::Document dom;
			dom.SetObject();
			if(error) {
				dom.AddMember(INCLUDED_NET_RESPONSE_ERROR, error, dom.GetAllocator());
			}
			
			return dom;
		}
		
		/**
		 * \brief Take in an array and output a ::rapidjson::Value array.
		 * 
		 * This deletes the array that is supplied.
		 *
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the size of the array is 0.
		 */
		template <typename T>
				inline ::rapidjson::Value rapid_json_array(const T* const items,
				const std::size_t size,
				::rapidjson::Document::AllocatorType& allocator) {
			#ifdef SABOT_THROW
			if(UNLIKELY(items == 0)) {
				throw std::invalid_argument(err_msg::_nllpntr);
			}
			if(UNLIKELY(size == 0)) {
				throw std::invalid_argument(err_msg::_zrlngth);
			}
			#endif
			
			::rapidjson::Value jArray(::rapidjson::kArrayType);
			for(std::size_t i = 0; i < size; i++) {
				jArray.PushBack(items[i], allocator);
			}
			delete[] items;
			return jArray;
		}
		
		/**
		 * \brief Common write code invariant of the result type.
		 */
		inline void _write(::rapidjson::Document& dom) {
			::rapidjson::Writer<::rapidjson::StringBuffer> writer(_jsonBuffer);
			dom.Accept(writer);
		}
	};
}

#endif
