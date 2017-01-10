#ifndef _STRING_BUFFER_HPP
#define _STRING_BUFFER_HPP

#include <common.hpp>
#include <managed_container.hpp>

/**
 * \brief The default size of the stack buffer.
 *
 * You should try to select the size to play nicely with the size of the cache line in the
 * context of the overall struct size.
 * 
 * \note Size in bytes.
 */
#define STRING_BUFFER_DEFAULT_STACK_SIZE 40

/**
 * \brief This is a lightweight buffer object used to hold results between computations.
 * 
 * This is designed to be as lightweight as possible. It has a hybrid memory model: if the
 * buffer size is within some given bounds, we store it directly on the stack, but it is
 * larger than these bounds or is initially within these bounds but grows to exceed them,
 * we move it to the heap. In this case, we waste a little bit of stack space, but the
 * point of the template parameter is to tune the buffer stack size so that this is a rare
 * occurrence. In this case, the heap mechanism doesn't come into play, but serves as a
 * backup.
 * 
 * The template parameter is the initial capacity in bytes to be used. If you use a
 * constructor accepting an existing buffer and the size is less than this initial
 * capacity, than the capacity is equal to or greater than the template parameter. If you
 * use a poorly chosen template parameter, i.e. 5 bytes but give an initial string of 10
 * bytes, the capacity will be equal to or greater than the size of the string. The
 * initial capacity either from the template parameter or size of the given data may be
 * rounded to a higher value for efficiency.
 */
template <std::uint64_t initialCapacity = STRING_BUFFER_DEFAULT_STACK_SIZE>
		struct string_buffer : public managed_container<char, std::uint64_t, initialCapacity> {
	
	using parentT = managed_container<char, std::uint64_t, initialCapacity>;

 public:
	/**
	 * \brief Empty constructor initializes buffer on stack with at least the given
	 * capacity.
	 */
	string_buffer()
			: parentT::managed_container() {
	}
	
	/**
	 * \brief String constructor initializes buffer on stack with at least the given
	 * capacity and copies the string into it.
	 * 
	 * If the string is larger than the given capacity, we allocate some heap memory to
	 * store it instead.
	 * 
	 * If a zero is passed as the str pointer we invoke the default constructor only. This
	 * is to keep compatability with how a raw cstring is used.
	 */
	string_buffer(const char* const str)
			: string_buffer() {
		if(str != 0) {
			this->add(str, strlen(str)+1);
		}
	}
	
	/**
	 * \brief Data buffer constructor initializes buffer on stack with at least the given
	 * capacity and copies the data into it.
	 * 
	 * If the data buffer is larger than the given capacity, we allocate some heap memory
	 * to store it instead.
	 */
	string_buffer(const char* const data, const std::uint64_t size)
			: string_buffer() {
		this->add(data, size);
	}
	
	/**
	 * \brief Copy constructor is disabled.
	 */
	string_buffer(const string_buffer&) = delete;
	
	/**
	 * \brief Move constructor.
	 */
	string_buffer(string_buffer&& old) = default;
	/**
	 * \brief Assignment operator is disabled.
	 */
	string_buffer& operator=(const string_buffer&) = delete;
	
	/**
	 * \brief Move assignment operator.
	 */
	string_buffer& operator=(string_buffer&& old) = default;
	
	/**
	 * \brief Destructor.
	 */
	~string_buffer() = default;
	
	/**
	 * \brief Concatenate a string to the buffer.
	 * 
	 * If there is already data within the buffer that ends with a '\0', then we remove
	 * it as to emulate string concatenation.
	 */
	void operator += (const char* const str) {
		if(this->count() != 0 && this->data()[this->count()-1] == '\0') {
			this->trim(1);
		}
		this->add(str, strlen(str)+1);
	}
	
	/**
	 * \brief Return the number of bytes stored.
	 * 
	 * Because our storage type is char, this is an alias to count().
	 */
	inline std::uint64_t size() const {
		return this->count();
	}
};

#endif
