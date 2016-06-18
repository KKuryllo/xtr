#pragma once
#include "VectorTools.h"
namespace xtr {
class IDList {
public:
  size_t add() {
    if (free_slots.empty()) {
      list_size++;
      return list_size - 1;
    } else {
      auto&& free_position = *(free_slots.end() - 1);
      free_slots.pop_back();
      return free_position;
    }
  }
  void remove(const size_t to_remove) {
    // Check if removing last element so that list may be collapsed to a smaller size
    if (to_remove == list_size - 1) {
      --list_size;
      // Check for additional free slots
      if (!free_slots.empty() && *free_slots.begin() == list_size - 1) {
        --list_size;
        // now iterater to remove all unecessary elements
        auto&& largest_free = free_slots.begin() + 1;
        while (largest_free != free_slots.end() && *largest_free == list_size - 1) {
          ++largest_free;		// shift last free slot backwards
          --list_size;		// decrease list size
        }
        // erase all free slots past end
        free_slots.erase(free_slots.begin(), largest_free);
      }
    }
    // Not removing last element so just add to list of free slots
    else insertOrderedVector(free_slots, to_remove, std::greater<size_t>());
  }
  inline size_t size() {
    return list_size;
  }
  inline size_t totalFree() {
    return free_slots.size();
  }
  inline bool empty() {
    return list_size == 0 ? true : false;
  }
  void clear() {
    free_slots.clear();
    list_size = 0;
  }

  std::vector<size_t>* getFreeSlots() { return &free_slots; };
  size_t* getListSize() { return &list_size; }

private:
  std::vector<size_t> free_slots;
  size_t list_size = 0;
};
}
