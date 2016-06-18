#pragma once
#include <vector>
#include "MutexedObject.h"
#include "IntAbbr.h"
#include "RefLink.h"
namespace xtr {
template<typename anchor, typename ST = int_abbr::ui16f>
class Linker {
	using Link = RefLink<Linker<anchor, ST>>;
public:
	using size_type = ST;
	using container = std::vector<size_type>;
	auto newLink(const size_type id)
	{
		{
			auto&& c = counter.Lock();
			if (id >= c->size()) c->resize(id + 1);
			c->at(id) = 0;
		}
		auto&& new_link = Link();
		new_link.unsafe_link(id);
		return std::move(new_link);
	}
	void link(const size_type id)
	{
    auto&& c = counter.Lock();
		++c->at(id);
	}
	void delink(const size_type id)
	{
		auto&& c = counter.Lock();
		assert(id < c->size());
		if (--c->at(id) == 0) {
			auto&& f = free_links.Lock();
			f->push_back(id);
		}
	}
	void getFreeLinks(container& swap_container){
		auto&& f = free_links.Lock();
		f->swap(swap_container);
		f->clear();
	}
	bool isSingle(const Link& test_link) {
		auto&& c = counter.Lock();
		if (c->at(test_link()) == 1) return true;
		else return false;
	}
private:
	MutexedObject<container> counter;
	MutexedObject<container> free_links;
	anchor a;
};
}
