/*
    wdb

    Copyright (C) 2014 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/

#ifndef LRULIST_H_
#define LRULIST_H_

#include <list>
#include <map>
#include <stdexcept>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <fstream>

/**
 * Map-like structure, with a fixed capacity. When max size has been reached,
 * the last accessed element will be evicted from this container.
 */
template<typename Key, typename Value>
class LRUList : boost::noncopyable
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef boost::shared_ptr<value_type> value_ptr;

	/**
	 * Construct the list, setting maximum number of elements.
	 */
	LRUList(unsigned capacity) : capacity_(capacity) {}

	/**
	 * Get the current number of elements.
	 */
	unsigned size() const;

	/**
	 * Get the highest number of elements to use
	 */
	unsigned maxSize() const;


	/**
	 * Get the value for the given key, or NULL if no such key in list
	 */
	value_ptr get(key_type key);

	/**
	 * Set key=value.
	 *
	 * @throws std::runtime_erorr if key already existed.
	 */
	void set(key_type key, value_ptr value);

	/**
	 * Set key=value. Will internally wrap the given pointer into a smart
	 * pointer, so the given object must react properly to being deleted -
	 * ie. it must have been allocated with new, and not explicitly deleted
	 * anywhere else.
	 *
	 * @throws std::runtime_erorr if key already existed.
	 * @return the set object, wrapped in a smart pointer
	 */
	value_ptr set(key_type key, value_type * value) {
		value_ptr ret(value);
		set(key, ret);
		return ret;
	}

private:
	typedef std::list<key_type> LRU;
	struct Data
	{
		typename LRU::iterator lru;
		value_ptr value;
	};
	typedef std::map<key_type, Data> UsageList;

	unsigned capacity_;
	UsageList inUse_;
	LRU lru_;
};




template<typename Key, typename Value>
typename LRUList<Key, Value>::value_ptr LRUList<Key, Value>::get(key_type key)
{
	typename UsageList::iterator find = inUse_.find(key);
	if ( find == inUse_.end() )
		return value_ptr();

	Data & data = find->second;
	lru_.splice(lru_.begin(), lru_, data.lru);

	assert(lru_.front() == key);

	return data.value;
}

template<typename Key, typename Value>
void LRUList<Key, Value>::set(key_type key, value_ptr value)
{
	if ( inUse_.find(key) != inUse_.end() )
		throw std::runtime_error("key already existed");

	if ( inUse_.size() >= capacity_ )
	{
		key_type deleteId = lru_.back();
		lru_.pop_back();
		inUse_.erase(deleteId);
	}

	lru_.push_front(key);

	Data & data = inUse_[key];
	data.value = value;
	data.lru = lru_.begin();
}

template<typename Key, typename Value>
unsigned LRUList<Key, Value>::size() const
{
	return inUse_.size();
}

template<typename Key, typename Value>
unsigned LRUList<Key, Value>::maxSize() const
{
	return capacity_;
}


#endif /* LRULIST_H_ */
