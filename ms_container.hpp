#ifndef _MS_CONTAINER_HPP
#define _MS_CONTAINER_HPP

#include <common.hpp>
#include "string_buffer.hpp"
#include <mutex>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

#define MS_CONTAINER_MSG_IDNOTFOUND "ID not found"
#define MS_CONTAINER_MSG_LABELNOTFOUND "Label not found"
#define MS_CONTAINER_MSG_LABELEXISTS "Label already exists"

/**
 * \brief A container item consisting an Id, a data object, and an optional
 * string label.
 */
template <typename T, typename U> struct ms_container_item {
 public:
	static_assert(std::is_arithmetic<T>::value, "T must be numeric type");
	
	/**
	 * \brief Constructor takes an id, a data object, and optionally a string
	 * label. The data object uses move semantics.
	 */
	ms_container_item(T id, U&& data, const char* const label = 0)
			:  _label(label), _id(id), _data(std::move(data)) {
	}
	
	/**
	 * \brief Copy constructor disabled.
	 */
	ms_container_item(const ms_container_item&) = delete;
	
	/**
	 * \brief Move constructor.
	 */
	ms_container_item(ms_container_item&& item)
			: _label(std::move(item._label)),
			_data(std::move(item._data)) {
		_id = item._id;
	}
	
	/**
	 * \brief Assignment operator disabled.
	 */
	ms_container_item& operator=(const ms_container_item&) = delete;
	
	/**
	 * \brief Move assignment operator.
	 */
	ms_container_item& operator=(ms_container_item&& item) {
		_id = item._id;
		_data = std::move(item._data);
		_label = std::move(item._label);
		item._label = 0;
		return *this;
	}
	
	/**
	 * \brief Get the id of the item.
	 */
	inline T id() const {
		return _id;
	}
	
	/**
	 * \brief Get the data of the item.
	 */
	inline U& data() {
		return _data;
	}
	
	/**
	 * \brief Get the label of the item.
	 */
	inline const char* label() const {
		return _label.data();
	}

 private:
	string_buffer<8> _label;
	T _id;
	U _data;
};

/**
 * \brief A container implementing move semantics for items with NO requirement of copy
 * constructor or assignment operator for items, unlike STL containers (map, vector, etc).
 * 
 * \warning If the uniqueLabel parameter is true, then each insert call will require
 * a search of every element stored to check if the new label already exists.
 * 
 * \warning Certain operations return a reference to an item stored within the container.
 * If you use such a function and perform operations on this reference without using the
 * returned mutexes, BAD THINGS WILL HAPPEN. See the documentation of the functions within
 * to prevent this.
 */
template <typename T, typename U, std::size_t initialCapacity,
		bool uniqueLabel = false> class ms_container {
 public:
	using id_t = T;
	using item_t = U;
	using item_type = ms_container_item<T, U>;
	using shared_lock = boost::upgrade_lock<boost::shared_mutex>;
	using upgrade_lock = boost::upgrade_to_unique_lock<boost::shared_mutex>;
	using unique_lock = boost::unique_lock<boost::shared_mutex>;
	
	/**
	 * \brief Constructor reserves space for items based upon the initial capacity.
	 */
	ms_container()
			: _capacity(initialCapacity),
			_size(0),
			_counter(1),
			_items(0) {
		_items = new item_type*[_capacity];
		_elementMutexes = new std::mutex*[_capacity];
		for(std::size_t i = 0; i < _capacity; i++) {
			_elementMutexes[i] = new std::mutex;
		}
	}
	
	/**
	 * \brief Copy constructor disabled.
	 */
	ms_container(const ms_container&) = delete;
	
	/**
	 * \brief Move constructor disabled.
	 */
	ms_container(ms_container&&) = delete;
	
	/**
	 * \brief Assignment operator disabled.
	 */
	ms_container& operator=(const ms_container&) = delete;
	
	/**
	 * \brief Move assignment operator disabled.
	 */
	ms_container& operator=(ms_container&&) = delete;
	
	/**
	 * \brief Destructor cleans up allocated memory.
	 */
	~ms_container() {
		clear();
		if(_items != 0) {
			delete[] _items;
		}
		for(std::size_t i = 0; i < _capacity; i++) {
			delete _elementMutexes[i];
		}
		delete[] _elementMutexes;
	}
	
	/**
	 * \brief Get an item by its id.
	 * 
	 * \warning Be sure to use the mutexes provided when calling, otherwise operations on
	 * the returned reference are NOT threadsafe!
	 * \throws std::out_of_range if id doesn't match any existing items.
	 * \remarks For search complexity, see find_index_by_id().
	 */
	const U& get(const T id, std::mutex*& elementMutex, boost::shared_mutex*& editMutex) {
		shared_lock lock(_editMutex);
		
		std::size_t index(0);
		
		if(find_index_by_id(id, index, lock)) {
			/** \todo: this should be locked here */
			elementMutex = _elementMutexes[index];
			editMutex = &_editMutex;
			return _items[index]->data();
		} else {
			throw std::out_of_range(MS_CONTAINER_MSG_IDNOTFOUND);
		}
	}
	
	/**
	 * \brief Get an item by its label.
	 *
	 * \warning Be sure to use the mutexes provided when calling, otherwise operations on
	 * the returned reference are NOT threadsafe!
	 * \throws std::out_of_range if id doesn't match any existing items.
	 * \remarks For search complexity is O(log_2(n)).
	 */
	const U& find(const char* const label, std::mutex*& elementMutex,
			boost::shared_mutex*& editMutex) {
		shared_lock lock(_editMutex);
		
		std::size_t index(0);
		
		if(find_index_by_label(label, index, lock)) {
			/** \todo: this should be locked here */
			elementMutex = _elementMutexes[index];
			editMutex = &_editMutex;
			return _items[index]->data();
		} else {
			throw std::out_of_range(MS_CONTAINER_MSG_LABELNOTFOUND);
		}
	}
	
	/**
	 * \brief Insert a new item amd return its ID.
	 * 
	 * \remarks For implementation, see overloaded insert().
	 */
	T insert(U&& item, const char* const label = 0) {
		shared_lock lock(_editMutex);
		
		// Compiler screws things up if we combine this into one line
		auto id = insert(lock, std::move(item), label);
		return _items[id]->id();
	}
	
	/**
	 * \brief Insert a new item and return a reference to the item.
	 * 
	 * This prevents a caller from having to insert and then search for the item.
	 * 
	 * \warning Be sure to use the mutexes provided when calling, otherwise operations on
	 * the returned reference are NOT threadsafe!
	 * \remarks For implementation, see overloaded insert().
	 */
	const U& insert_and_return(U&& item, std::mutex*& elementMutex,
			boost::shared_mutex*& editMutex, const char* const label = 0) {
		shared_lock lock(_editMutex);
		
		auto index(insert(lock, std::move(item), label));
		/** \todo: this should be locked here */
		elementMutex = _elementMutexes[index];
		editMutex = &_editMutex;
		return _items[index]->data();
	}
	
	/**
	 * \brief Insert a new item and return a reference to the item along with its id.
	 * 
	 * This prevents a caller from having to insert and then search for the item.
	 * 
	 * \warning Be sure to use the mutexes provided when calling, otherwise operations on
	 * the returned reference are NOT threadsafe!
	 * \remarks For implementation, see overloaded insert().
	 */
	const std::pair<T, U&> insert_and_return_full(U&& item, std::mutex*& elementMutex,
			boost::shared_mutex*& editMutex, const char* const label = 0) {
		shared_lock lock(_editMutex);
		
		auto index(insert(lock, std::move(item), label));
		/** \todo: this should be locked here */
		elementMutex = _elementMutexes[index];
		editMutex = &_editMutex;
		return std::pair<T, U&>(_items[index]->id(), _items[index]->data());
	}
	
	/**
	 * \brief Get an item by its id to edit.
	 * 
	 * \warning Be sure to use the mutexes provided when calling, otherwise operations on
	 * the returned reference are NOT threadsafe!
	 * \throws std::out_of_range if ID doesn't match any existing items.
	 * \remarks For search complexity, see find_index_by_id().
	 */
	U& edit(const T id, std::mutex*& elementMutex, boost::shared_mutex*& editMutex) {
		shared_lock lock(_editMutex);
		
		std::size_t index(0);
		
		if(find_index_by_id(id, index, lock)) {
			/** \todo: this should be locked here */
			elementMutex = _elementMutexes[index];
			editMutex = &_editMutex;
			return _items[index]->data();
		} else {
			throw std::out_of_range(MS_CONTAINER_MSG_IDNOTFOUND);
		}
	}
	
	/**
	 * \brief Delete an item by ID.
	 *
	 * \remarks For search complexity, see find_index_by_id().
	 */
	bool erase(const T id) {
		shared_lock lock(_editMutex);
		
		std::size_t index(0);
		
		if(find_index_by_id(id, index, lock)) {
			upgrade_lock uniqueLock(lock);
			
			delete _items[index];
			_items[index] = 0;
			
			// not ideal solution but it works for now
			memmove(&_items[index], &_items[index+1], sizeof(item_type*)*(_size-index));
			_items[_size-1] = 0;
			_size--;
			
			//shrink_to_fit(uniqueLock);
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 * \brief Delete all items stored within the container.
	 */
	inline void clear() {
		shared_lock lock(_editMutex);
		
		for(std::size_t i = 0; i < _size; i++) {
			delete _items[i];
			_items[i] = 0;
		}
	}
	
	/**
	 * \brief Get the number of items stored.
	 */
	inline std::size_t size() {
		shared_lock lock(_editMutex);
		
		return _size;
	}
	
	/**
	 * \brief Get the maximum number of items that may be stored at the current capacity.
	 */
	inline std::size_t capacity() {
		shared_lock lock(_editMutex);
		
		return _capacity;
	}
	
 private:
	std::size_t _capacity;
	std::size_t _size;
	std::size_t _counter;
	
	std::uint64_t _biggestIndex; // The biggest nonzero index
	std::uint64_t _gapFactor; // The number of gaps between 0 and biggestIndex
	
	item_type** _items;
	std::mutex** _elementMutexes;
	boost::shared_mutex _editMutex;
	
	/**
	 * \brief Insert an item into the container.
	 */
	std::size_t insert(shared_lock& lock, U&& item, const char* const label = 0) {
		if(uniqueLabel && label != 0) {
			std::size_t index(0);
			if(find_index_by_label(label, index, lock)) {
				throw std::invalid_argument(MS_CONTAINER_MSG_LABELEXISTS);
			}
		}
		
		upgrade_lock upgradeLock(lock);
		
		if(_size == _capacity) {
			double_capacity(upgradeLock);
		}
		
		auto index(_size);
		_items[index] = new item_type(_counter++, std::move(item), label);
		_size++;
		return index;
	}
	
	/**
	 * \brief Compress the items in the container so there are no empty spaces.
	 * 
	 * Wrapper for threads holding upgrade_lock.
	 * 
	 * \see For implementation, see _shrink_to_fit().
	 */
	inline void shrink_to_fit(upgrade_lock& upgradeLock) {
		UNUSED(upgradeLock);
		_shrink_to_fit();
	}
	
	/**
	 * \brief Compress the items in the container so there are no empty spaces.
	 * 
	 * Wrapper for threads holding unique_lock.
	 * 
	 * \see For implementation, see _shrink_to_fit().
	 */
	inline void shrink_to_fit(unique_lock& uniqueLock) {
		_shrink_to_fit();
	}
	
	/**
	 * \brief Compress the items in the container so there are no empty spaces.
	 *
	 * \warning Not thread safe.
	 */
	inline void _shrink_to_fit(const T emptyIndex) {
		auto temp(_size);
		
		for(std::size_t i = 0; i < temp; i++) {
			if(_items[i] == 0) {
				for(std::size_t j = 1; j < temp; j++) {
					_items[i+j-1] = _items[i+j];
					_elementMutexes[i+j-1] = _elementMutexes[i+j];
				}
				--temp;
			}
		}
		
		_size = temp;
	}
	
	/**
	 * \brief Double the current capacity of the container.
	 * 
	 * Wrapper for threads holding upgrade_lock.
	 * 
	 * \see For implementation, see _double_capacity().
	 */
	inline void double_capacity(upgrade_lock& upgradeLock) {
		UNUSED(upgradeLock);
		_double_capacity();
	}
	
	/**
	 * \brief Double the current capacity of the container.
	 * 
	 * Wrapper for threads holding unique_lock.
	 * 
	 * \see For implementation, see _double_capacity().
	 */
	inline void double_capacity(unique_lock& uniqueLock) {
		UNUSED(uniqueLock);
		_double_capacity();
	}
	
	/**
	 * \brief Double the current capacity of the container.
	 *
	 * \warning Not thread safe.
	 */
	inline void _double_capacity() {
		assert(_capacity != 0);
		
		auto oldCapacity = _capacity;
		_capacity *= 2;
		if(_size != 0) {
			assert(_items != 0);
			assert(_elementMutexes != 0);
			
			// Elements
			auto newList = new item_type*[_capacity];
			std::copy(_items, _items + _size, newList);
			delete[] _items;
			_items = newList;
			
			// Element Mutexes
			auto newMutexList = new std::mutex*[_capacity];
			std::copy(_elementMutexes, _elementMutexes + _size, newMutexList);
			delete[] _elementMutexes;
			_elementMutexes = newMutexList;
			for(std::size_t i = oldCapacity; i < _capacity; i++) {
				_elementMutexes[i] = new std::mutex;
			}
		}
	}
	
	/**
	 * \brief Find the index of an item by its id.
	 * 
	 * Wrapper for threads holding a shared_lock.
	 * 
	 * \see For implementation, see _find_index_by_id().
	 */
	inline bool find_index_by_id(const std::size_t id, std::size_t& index,
			shared_lock& lock) const {
		UNUSED(lock);
		return _find_index_by_id(id, index);
	}
	
	/**
	 * \brief Find the index of an item by its id.
	 * 
	 * Wrapper for threads holding a upgrade_lock.
	 * 
	 * \see For implementation, see _find_index_by_id().
	 */
	inline bool find_index_by_id(const std::size_t id, std::size_t& index,
			upgrade_lock& lock) const {
		UNUSED(lock);
		return _find_index_by_id(id, index);
	}
	
	/**
	 * \brief Find the index of an item by its id.
	 * 
	 * Wrapper for threads holding a unique_lock.
	 * 
	 * \see For implementation, see _find_index_by_id().
	 */
	inline bool find_index_by_id(const std::size_t id, std::size_t& index,
			unique_lock& lock) const {
		UNUSED(lock);
		return _find_index_by_id(id, index);
	}
	
	/**
	 * \brief Internal method, find the index of an item by its id.
	 * 
	 * \warning Not thread safe.
	 * \remarks Uses binary search, complexity is O(log_2(n)).
	 */
	inline bool _find_index_by_id(const std::size_t id, std::size_t& index) const {
		assert(id != 0);
		
		if(_size != 0) {
			std::int_fast64_t left(0);
			std::int_fast64_t right(_size-1);
			
			while(left <= right) {
				std::uint_fast64_t middle((left+right)/2);
				if(_items[middle]->id() > id) {
					// Search left
					right = middle - 1;
				} else if(_items[middle]->id() < id) {
					// Search right
					left = middle + 1;
				} else {
					index = middle;
					return true;
				}
			}
		}
		
		return false;
	}
	
	/**
	 * \brief Find the index of an item by its label.
	 * 
	 * Wrapper for threads holding a shared_lock.
	 * 
	 * \see For implementation, see _find_index_by_label().
	 */
	inline bool find_index_by_label(const char* const needle, std::size_t& index,
			shared_lock& lock) const {
		UNUSED(lock);
		return _find_index_by_label(needle, index);
	}
	
	/**
	 * \brief Find the index of an item by its label.
	 * 
	 * Wrapper for threads holding an upgrade_lock.
	 * 
	 * \see For implementation, see _find_index_by_label().
	 */
	inline bool find_index_by_label(const char* const needle, std::size_t& index,
			upgrade_lock& lock) const {
		UNUSED(lock);
		return _find_index_by_label(needle, index);
	}
	
	/**
	 * \brief Find the index of an item by its label.
	 * 
	 * Wrapper for threads holding an unique_lock.
	 * 
	 * \see For implementation, see _find_index_by_label().
	 */
	inline bool find_index_by_label(const char* const needle, std::size_t& index,
			unique_lock& lock) const {
		return _find_index_by_label(needle, index);
	}
	
	/**
	 * \brief Find the index of an item by its label.
	 * 
	 * \warning Not thread safe.
	 * \remarks Uses linear search, complexity is O(n).
	 */
	inline bool _find_index_by_label(const char* const needle, std::size_t& index)
			const {
		assert(needle != 0);
		
		index = 0;
		while(index < _size) {
			if(_items[index] != 0 && strcmp(_items[index]->label(), needle) == 0) {
				return true;
			}
			++index;
		}
		return false;
	}
};

#endif
