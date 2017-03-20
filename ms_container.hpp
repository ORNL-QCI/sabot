#ifndef _MS_CONTAINER_HPP
#define _MS_CONTAINER_HPP

#include <common.hpp>
#include "string_buffer.hpp"
#include <atomic>
#include <map>
#include <type_traits>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

/**
 * \brief A thread-safe sorted associated container with unique keys.
 * 
 * The list itself is threadsafe, i.e. multiple threads may add and remove items
 * from the list. Each element is threadsafe, i.e. multiple threads may modify
 * the same item. These two are mutually exclusive however, such that one thread
 * may not be modifying an item while the list is being resized.
 * 
 * \warning Currently only works for dynamic objects
 * \todo add move semantics back in
 * \todo atomic_flag allocation could be improevd
 * \todo Custom deallocator for items, keys
 * 
 */
template <typename Key,
          typename T,
          typename Compare = std::less<Key>>
        class ms_container
        : protected std::map<Key, std::pair<std::atomic_flag*, T*>, Compare> {
 private:
    using base_t = std::map<Key, std::pair<std::atomic_flag*, T*>, Compare>;
    using list_shared_lock_t = boost::shared_lock<boost::shared_mutex>;
    using list_unique_lock_t = boost::unique_lock<boost::shared_mutex>;
    
 public:
    ms_container() {
        initialize_keygenerator();
    }
        
    ~ms_container() {
        for(auto& item : *this) {
            delete item.second.first;
            delete item.second.second;
        }
    }
    
    template <bool _isInt = std::is_integral<Key>::value,
             typename std::enable_if<_isInt, bool>::type = true>
            Key insert(T *item) {
        list_unique_lock_t lock(_listMutex);
        Key ret = get_key();
        
        auto temp = typename base_t::value_type(ret, typename base_t::mapped_type(new std::atomic_flag, item));
        temp.second.first->clear(std::memory_order_release);
        static_cast<base_t*>(this)->insert(this->end(), temp);
        return ret;
    }
    
    template <typename F,
              bool _isInt = std::is_integral<Key>::value,
              typename std::enable_if<_isInt, bool>::type = true>
            Key insert(F f) {
        list_unique_lock_t lock(_listMutex);
        Key ret = get_key();
        
        auto temp = typename base_t::value_type(ret, typename base_t::mapped_type(new std::atomic_flag, f()));
        temp.second.first->clear(std::memory_order_release);
        static_cast<base_t*>(this)->insert(this->end(), temp);
        return ret;
    }
    
    template <bool _isStr = std::is_same<std::string, Key>::value,
              typename std::enable_if<_isStr, bool>::type = true>
            bool insert(Key key, T *item) {
        list_unique_lock_t lock(_listMutex);
        auto temp = typename base_t::value_type(key, typename base_t::mapped_type(new std::atomic_flag, item));
        temp.second.first->clear(std::memory_order_release);
        auto test = static_cast<base_t*>(this)->insert(temp);
        return test.second;
    }
    
    template <typename F,
              bool _isStr = std::is_same<std::string, Key>::value,
              typename std::enable_if<_isStr, bool>::type = true>
            bool insert(Key key, F f) {
        list_unique_lock_t lock(_listMutex);
        auto temp = typename base_t::value_type(key, typename base_t::mapped_type(new std::atomic_flag, f()));
        temp.second.first->clear(std::memory_order_release);
        auto test = static_cast<base_t*>(this)->insert(temp);
        return test.second;
    }
    
    auto erase(const Key& key) -> decltype(this->erase(key)) {
        list_unique_lock_t lock(_listMutex);
        
        auto it = static_cast<base_t*>(this)->find(key);
        delete it->second.first;
        delete it->second.second;
        /** \todo Stop double search */
        return static_cast<base_t*>(this)->erase(key);
    }
    
    auto count(Key key)
            -> decltype(static_cast<base_t*>(this)->count(key)) {
        list_shared_lock_t lock(_listMutex);
        return static_cast<base_t*>(this)->count(key);
    }
    
    auto size() -> decltype(this->size()) {
        list_shared_lock_t lock(_listMutex);
        return static_cast<base_t*>(this)->size();
    }
    
    template<typename F>
            auto edit_item(Key key, F f) -> decltype(f(*(this->find(key)->second.second))) {
        list_shared_lock_t lock(_listMutex);
        
        auto item = static_cast<base_t*>(this)->find(key);
        /** \todo error handle */
        if(item == this->end()) throw std::invalid_argument("not found");
        
        // We suppose that the resource will very rarely be used by another
        // thread, and therefore the spinlock provides better performance in the
        // average case. However, if we had to wait for the resource to become
        // free, this would limit performance.
        while(item->second.first->test_and_set(std::memory_order_acquire)) {}
        auto ret = f(*(item->second.second));
        item->second.first->clear(std::memory_order_release);
        return ret;
    }
    
 private:
    boost::shared_mutex _listMutex;
    
    Key _keyGenerator;
    
    template <bool _isInt = std::is_integral<Key>::value,
              typename std::enable_if<_isInt, bool>::type = true>
            inline void initialize_keygenerator() noexcept {
        _keyGenerator = 0;
    }
    
    template <bool _isInt = std::is_integral<Key>::value,
              typename std::enable_if<!_isInt, bool>::type = true>
            inline void initialize_keygenerator() noexcept {
    }
    
    /**
     * \bug If keyGenerator wraps, we can't guarentee uniqueness.
     */
    template <bool _isInt = std::is_integral<Key>::value,
              typename std::enable_if<_isInt, bool>::type = true>
            inline Key get_key() noexcept {
        return _keyGenerator++;
    }
};
#endif
