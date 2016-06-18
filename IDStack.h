#pragma once
#include "VectorTools.h"
namespace xtr {
template <typename T>
class IDStack {
public:
	typedef typename std::vector<T>::size_type size_type;
	inline T& operator[](const size_t index)
	{
		return IDs[index];
	}
	inline const T& operator[](const size_t index) const
	{
		return IDs[index];
	}
	template <typename T2>
	size_t add(T2&& new_item)
	{
		if (free_slots.empty()) {
			IDs.push_back(std::move(new_item));
			return IDs.size() - 1;
		} else {
			size_t free_position = *(free_slots.end() - 1);
			IDs[free_position] = std::move(new_item);
			free_slots.pop_back();
			return free_position;
		}
	}
	bool isFree(const size_t position) const
	{
		if (position >= size()) return true;
		auto&& result = std::lower_bound(free_slots.begin(), free_slots.end(), position, std::greater<size_t>());
		if (result == free_slots.end() || *result != position)
			return false;
		else return true;
	}
	template <typename T2>
	bool addAt(T2&& new_item, const size_t position)
	{
		// action depends on whether greate or less than size
		if (position >= size()) {		// expand list size and add item to position
			auto&& old_size = size();
			IDs.resize(position + 1);
			IDs[position] = new_item;
			// place all interveening items in the removed list
			for (int i = old_size; i < size() - 1; ++i)			//TODO This can be optimized by shifting the vector to the right by size changed and then inserting to the front by largest element first
				insertOrderedVector(free_slots, i, std::greater<size_t>());
		} else {		// try to insert at a free slot if possible
			auto&& result = std::lower_bound(free_slots.begin(), free_slots.end(), position, std::greater<size_t>());
			if (result != free_slots.end() || *result != position) {	// a free slot is available so add there
				ID[*result] = std::move(new_item);
				free_slots.erase(result);
			} else return false;		// no slot available return false
		}

	}
	void remove(const size_t to_remove)
	{
		// Check if removing last element so that list may be collapsed to a smaller size
		if (to_remove == IDs.size() - 1) {
			IDs.pop_back();
			if (!free_slots.empty() && *free_slots.begin() == IDs.size() - 1) {
				IDs.pop_back();
				// now iterater to remove all unecessary elements
				auto largest_free = free_slots.begin() + 1;
				while (largest_free != free_slots.end() && *largest_free == IDs.size() - 1) {
					++largest_free;		// shift last free slot backwards
					IDs.pop_back();	// decrease list size
				}
				// erase all free slots past end
				free_slots.erase(free_slots.begin(), largest_free);
			}
		}
		// Not removing last element so just add to list of free slots
		else insertOrderedVector(free_slots, to_remove, std::greater<size_t>());
	}
	void clear()
	{
		free_slots.clear();
		IDs.clear();
	}
	inline std::vector<T>& data()
	{
		return IDs;
	}
	inline size_t size() const
	{
		return IDs.size();
	}
	inline bool empty() const
	{
		return IDs.empty();
	}
private:
	std::vector<size_t> free_slots;
	std::vector<T> IDs;
};
}
