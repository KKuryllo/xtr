#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdint>
#include "VectorTools.h"
namespace xtr {
template<typename KeyT, typename T, typename size_spec = std::uintptr_t>
class VectorMap {
public:
	using size_type = size_spec;
	using key_pair = std::pair<KeyT, size_type>;	
	struct Result {
		Result(const size_type index_, T& value_, bool not_new_) : index(index_), value(value_), not_new(not_new_) {}
		operator bool() const
		{
			return not_new;
		}
		const size_type index;
		T& value;
		const bool not_new;
	};
	VectorMap() {}
	~VectorMap() { delete[] data; data = nullptr; }
	T& operator[](const size_type index)
	{
		assert(index < size());
		return data[index];
	}
	void removeKey(const KeyT& key)
	{
		auto position = std::lower_bound(table.begin(), table.end(), key, [](const key_pair& pair, const KeyT& key) { return pair.first < key; });
		assert(position != table.end() && position->first == key);
		remove(position->second);
		table.erase(position);
	}
	void removeIndex(const size_type to_remove)
	{
		// First find Key
		auto&& position = std::find_if(table.begin(), table.end(), [&](const key_pair& pair) { return pair.second == to_remove; });
		assert(position != table.end());
		table.erase(position);	
		remove(to_remove);
	}
	Result validate(const KeyT& key)
	{
		auto position = std::lower_bound(table.begin(), table.end(), key, [](const key_pair& pair, const KeyT& key) { return pair.first < key; });
		if (position == table.end() || position->first != key) {	// insert a new value
			size_type new_index = add();
			table.insert(position, std::make_pair(key, new_index));
			return Result(new_index, data[new_index], false );
		} else return Result(position->second, data[position->second], true);
	}
	size_type size() const
	{
		return end_ - data;
	}
	size_type capacity() const
	{
		return capacity_ - data;
	}
	T* begin() { return data; }
	T* end() { return end_; }
private:
	void remove(const size_t to_remove)
	{
		// Check if removing last element so that list may be collapsed to a smaller size
		if (to_remove == size() - size_type(1)) {
			--end_;
			if (!free_slots.empty() && *free_slots.begin() == size() - size_type(1)) { // if the preceeding slot was empty begin colapse to smaller size
				--end_;
				// now iterater to remove all unecessary elements
				auto largest_free = free_slots.begin() + size_type(1);
				while (largest_free != free_slots.end() && *largest_free == size() - size_type(1)) {
					++largest_free;		// shift last free slot backwards
					--end_;	// decrease list size
				}
				// erase all free slots past end
				free_slots.erase(free_slots.begin(), largest_free);
			}
		}
		// Not removing last element so just add to list of free slots
		else {
			auto&& result = insertOrderedVector(free_slots, to_remove, std::greater<size_t>());
			assert(result);
		}
	}
	const size_type add()
	{
		if (free_slots.empty()) {
			return appendNew();
		} else {
			size_type free_position = *(free_slots.end() - size_type(1));
			free_slots.pop_back();
			return free_position;
		}
	}
	const size_type appendNew(){
		auto new_size = size() + size_type(1);
		if (new_size > capacity()) {
			auto new_capacity = new_size * size_type(2);
			auto new_data = new T[new_capacity];
			auto new_iter = &new_data[0];
			// copy over all data
			for (auto i = data; i < end_; ++i) {
				*new_iter = *i;
				++new_iter;
			}
			delete[] data;
			data = new_data;
			end_ = data + new_size;
			capacity_ = data + new_capacity;
			return new_size - size_type(1);
		}
		else {
			++end_;
			return size() - size_type(1);
		}
	}
	T* data = nullptr;
	T* end_ = nullptr;
	T* capacity_ = nullptr;
	std::vector<key_pair> table;
	std::vector<size_type> free_slots;
};
}
