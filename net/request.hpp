#ifndef _NET_REQUEST_HPP
#define _NET_REQUEST_HPP

#include <common.hpp>
#include <rapidjson/document.h>

/**
 * \brief The string for the method field.
 */
#define INCLUDED_NET_REQUEST_METHOD "method"

/**
 * \brief The string for the parameters field.
 */
#define INCLUDED_NET_REQUEST_PARAMETER "parameters"

namespace net {
	/**
	 * \brief A JSON request from the client.
	 * 
	 * All requests are UTF-8.
	 */
	struct request {
	 public:
		/**
		 * \brief Decoding constructor takes in a mutable cstring that contains our JSON.
		 * 
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::invalid_argument if a null pointer is supplied.
		 * \throws Throws std::invalid_argument if the JSON string does not have a method
		 * field.
		 * \throws Throws std::invalid_argument if the JSON string does not have a
		 * parameters field.
		 */
		request(char* str) {
			#ifdef SABOT_THROW
			if(UNLIKELY(str == 0)) {
				throw std::invalid_argument(err_msg::_nllpntr);
			}
			#endif
			
			_dom.ParseInsitu(str);
			
			#ifdef SABOT_THROW
			if(UNLIKELY(!_dom.HasMember(INCLUDED_NET_REQUEST_METHOD))) {
				throw std::invalid_argument(err_msg::_tpntfnd);
			}
			if(UNLIKELY(!_dom.HasMember(INCLUDED_NET_REQUEST_PARAMETER))) {
				throw std::invalid_argument(err_msg::_tpntfnd);
			}
			#endif
		}
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		request(const request&) = delete;
		
		/**
		 * \brief Move constructor.
		 */
		request(request&& old)
				: _dom(std::move(old._dom)) {
		}
		
		/**
		 * \brief Assignment operator is disabled.
		 */
		request& operator=(const request&) = delete;
		
		/**
		 * \brief Move assignment operator.
		 */
		request& operator=(request&& old) {
			_dom = std::move(old._dom);
			
			return *this;
		}
		
		/**
		 * \brief Destructor.
		 */
		~request() {
		}
		
		/**
		 * \brief Return the method name.
		 */
		inline const char* method() const {
			return _dom[INCLUDED_NET_REQUEST_METHOD].GetString();
		}
		
		/**
		 * \brief Return the length of the method.
		 * 
		 * Not including null terminator.
		 */
		inline std::size_t method_size() const {
			return _dom[INCLUDED_NET_REQUEST_METHOD].GetStringLength();
		}
		
		/**
		 * \brief Return the number of parameters.
		 */
		inline std::size_t parameter_count() const {
			return _dom[INCLUDED_NET_REQUEST_PARAMETER].Size();
		}
		
		/**
		 * \brief Return a packed representation of the method attributes.
		 * 
		 * \note This function only throws if the proper preprocessor macro is enabled.
		 * 
		 * \throws Throws std::runtime_error if the length of the method name is greater
		 * than 31.
		 * \throws Throws std::runtime_error if the number of parameters is greater than
		 * 7.
		 */
		inline unsigned char method_attr() const {
			#ifdef SABOT_THROW
			if(UNLIKELY(method_size() > 0x1F)) {
				throw std::runtime_error(err_msg::_badvals);
			}
			if(UNLIKELY(parameter_count() > 0x07)) {
				throw std::runtime_error(err_msg::_badvals);
			}
			#endif
			
			return method_size() + (parameter_count() << 5);
		}
		
		/**
		 * \brief Return a type T parameter by index.
		 */
		template <typename T> inline T parameter(const std::size_t idx) const;
	
	 private:
		/**
		 * \brief The rapidjson DOM which contains parsed values.
		 * 
		 * Uses UTF-8 by default.
		 */
		::rapidjson::Document _dom;
	};
	
	/**
	 * \brief Return a cstring parameter by index.
	 */
	template <> inline const char*
			request::parameter<const char*>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetString();
	}
	
	/**
	 * \brief Return a cstring array parameter by index.
	 */
	template <> inline const char**
			request::parameter<const char**>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new const char*[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetString();
		}
		
		return array;
	}
	
	/**
	 * \brief Return a cstring array parameter by index.
	 */
	template <> inline const char* const*
			request::parameter<const char* const*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new const char*[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetString();
		}
		
		return array;
	}
	
	/**
	 * \brief Return a bool parameter by index.
	 */
	template <> inline bool
			request::parameter<bool>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetBool();
	}
	
	/**
	 * \brief Return a bool array parameter by index.
	 */
	template <> inline bool*
			request::parameter<bool*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new bool[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetBool();
		}
		
		return array;
	}
	
	/**
	 * \brief Return a char parameter by index.
	 */
	template <> inline char
			request::parameter<char>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetString()[0];
	}
	
	/**
	 * \brief Return an unsigned short parameter by index.
	 * 
	 * \warning Possible loss of precision.
	 */
	template <> inline unsigned short
			request::parameter<unsigned short>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetUint();
	}
	
	/**
	 * \brief Return an unsigned short array parameter by index.
	 * 
	 * \warning Possible loss of precision.
	 */
	template <> inline unsigned short*
			request::parameter<unsigned short*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new unsigned short[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetUint();
		}
		
		return array;
	}
	
	/**
	 * \brief Return a short parameter by index.
	 * 
	 * \warning Possible loss of precision.
	 */
	template <> inline short
			request::parameter<short>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetInt();
	}
	
	/**
	 * \brief Return a short array parameter by index.
	 * 
	 * \warning Possible loss of precision.
	 */
	template <> inline short*
			request::parameter<short*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new short[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetInt();
		}
		
		return array;
	}
	
	/**
	 * \brief Return an unsigned int parameter by index.
	 */
	template <> inline unsigned int
			request::parameter<unsigned int>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetUint();
	}
	
	/**
	 * \brief Return an unsigned int array parameter by index.
	 */
	template <> inline unsigned int*
			request::parameter<unsigned int*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new unsigned int[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetUint();
		}
		
		return array;
	}
	
	/**
	 * \brief Return an int parameter by index.
	 */
	template <> inline int
			request::parameter<int>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetInt();
	}
	
	/**
	 * \brief Return an int array parameter by index.
	 */
	template <> inline int*
			request::parameter<int*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new int[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetInt();
		}
		
		return array;
	}
	
	/**
	 * \brief Return an unsigned long int parameter by index.
	 */
	template <> inline unsigned long int
			request::parameter<unsigned long int>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetUint64();
	}
	
	/**
	 * \brief Return an unsigned long int array parameter by index.
	 */
	template <> inline unsigned long int*
			request::parameter<unsigned long int*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new unsigned long int[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetUint64();
		}
		
		return array;
	}
	
	/**
	 * \brief Return a long int parameter by index.
	 */
	template <> inline long int
			request::parameter<long int>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetInt64();
	}
	
	/**
	 * \brief Return a long int array parameter by index.
	 */
	template <> inline long int*
			request::parameter<long int*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new long int[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetInt64();
		}
		
		return array;
	}
	
	/**
	 * \brief Return a float parameter by index.
	 */
	template <> inline float
			request::parameter<float>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetDouble();
	}
	
	/**
	 * \brief Return a float array parameter by index.
	 */
	template <> inline float*
			request::parameter<float*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new float[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetDouble();
		}
		
		return array;
	}
	
	/**
	 * \brief Return a double parameter by index.
	 */
	template <> inline double
			request::parameter<double>(const std::size_t idx) const {
		return _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].GetDouble();
	}
	
	/**
	 * \brief Return a double array parameter by index.
	 */
	template <> inline double*
			request::parameter<double*>(const std::size_t idx) const {
		auto size = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx].Size();
		auto array = new double[size];
		for(std::size_t i = 0; i < size; i++) {
			array[i] = _dom[INCLUDED_NET_REQUEST_PARAMETER][idx][i].GetDouble();
		}
		
		return array;
	}
}

#endif
