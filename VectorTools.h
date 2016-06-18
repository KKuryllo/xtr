#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace xtr {
	template<class vectorType>
	// This template is used to remove an object from a vector by a name variable of type std::string contained in the object
	// It returns true if the object is found
	void appendVector(vectorType& destination, const vectorType& source){
		destination.insert(destination.end(), source.begin(), source.end());
	}

	template<class entry>
	// insert an item into an ordered vector if not already in vector and return true if inserted
	bool insertOrderedVector(std::vector<entry>& container, const entry&& item){
		auto&& result = std::lower_bound(container.begin(), container.end(), item);
		if (result == container.end() || *result != item) {
			container.insert(result, item);
			return true;
		}
		else return false;
	}

	template<class entry>
	// insert an item into an ordered vector if not already in vector and return true if inserted
	bool insertOrderedVector(std::vector<entry>& container, const entry& item){
		auto&& result = std::lower_bound(container.begin(), container.end(), item);
		if (result == container.end() || *result != item) {
			container.insert(result, item);
			return true;
		}
		else return false;
	}

	template<class entry, typename pred>
	// insert an item into an ordered vector if not already in vector and return true if inserted
	bool insertOrderedVector(std::vector<entry>& container, const entry&& item, const pred& predicate){
		auto&& result = std::lower_bound(container.begin(), container.end(), item, predicate);
		if (result == container.end() || *result != item) {
			container.insert(result, item);
			return true;
		}
		else return false;
	}

	template<class entry, typename pred>
	// insert an item into an ordered vector if not already in vector and return true if inserted
	bool insertOrderedVector(std::vector<entry>& container, const entry& item, const pred& predicate){
		auto&& result = std::lower_bound(container.begin(), container.end(), item, predicate);
		if (result == container.end() || *result != item) {
			container.insert(result, item);
			return true;
		}
		else return false;
	}

	template<class entry>
	// insert an item into an ordered vector if not already in vector and return reference to item
	entry& insertOrderedVectorReturn(std::vector<entry>& container, entry&& item){
		auto&& result = std::lower_bound(container.begin(), container.end(), item);
		if (result == container.end() || *result != item)
			return *container.insert(result, item);
		else return *result;
	}

	template<class entry, typename pred >
	// insert an item into an ordered vector if not already in vector and return reference to item
	entry& insertOrderedVectorReturn(std::vector<entry>& container, entry&& item, const pred& predicate){
		auto&& result = std::lower_bound(container.begin(), container.end(), item, predicate);
		if (result == container.end() || *result != item)
			return *container.insert(result, item);
		else return *result;
	}

	template<class entry>
	// insert an item into an ordered vector if already ther override the previous item
	void insertOrderedOverRide(std::vector<entry>& container, entry& item){
		auto&& result = std::lower_bound(container.begin(), container.end(), item);
		if (result == container.end() || *result != item)
			container.insert(result, item);
		else
			*result = item;
	}

	template<class entry>
	// insert an item into an ordered vector if already ther override the previous item
	void insertOrderedOverRide(std::vector<entry>& container, entry&& item){
		auto&& result = std::lower_bound(container.begin(), container.end(), item);
		if (result == container.end() || *result != item)
			container.insert(result, item);
		else
			*result = item;
	}

	template<class pointerType>
	// This template is used to remove an object from a vector by the object itself
	// It returns true if the object is removed
	bool removeItem(pointerType object, std::vector<pointerType>& container){
		auto&& find_result = std::find(container.begin(), container.end(), object);
		if (find_result != container.end()){
			container.erase(find_result);
			return true;
		}
		return false;
	}

	template<class pointerType, typename pred>
	// This template is used to remove vector by a uninary predicate
	// It returns true if the object is removed
	bool removeItem(std::vector<pointerType>& container, const pred&& predicate)
	{
		auto&& find_result = std::find_if(container.begin(), container.end(), predicate);
		if (find_result != container.end()) {
			container.erase(find_result);
			return true;
		}
		return false;
	}

	template<class pointerType, typename pred>
	// This template is used to remove a pointer vector by a uninary predicate
	// It returns the pointer or nullptr if it doesn't exits
	pointerType removeItemReturn(std::vector<pointerType>& container, const pred&& predicate)
	{
		auto&& find_result = std::find_if(container.begin(), container.end(), predicate);
		if (find_result != container.end()) {
			pointerType ret_val = *find_result;
			container.erase(find_result);
			return ret_val;
		}
		return nullptr;
	}

	template<class pointerType >
	// This template is used to remove an object from a vector by a name variable of type std::string contained in the object
	// It returns true if the object is found
	bool removeItem(const std::string& name, std::vector<pointerType>& container){
		for (auto&& iter = container.begin(); iter != container.end(); ++iter)
			if ((*iter)->name == name){
				container.erase(iter);
				return true;
			}
		return false;
	}

	template<class pointerType>
	// This template is used to retrieve an object from a vector by a name variable of type std::string contained in the object
	pointerType getItem(const std::string& name, const std::vector<pointerType>& container){
		for (auto iter : container)
			if (iter->name == name)
				return iter;
		return nullptr;
	}
}

