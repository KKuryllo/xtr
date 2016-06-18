#pragma once
namespace xtr {
template<typename L>
class RefLink {
public:
	using size_type = typename L::size_type;
	RefLink() : id_(0) {}
	RefLink(RefLink<L>&& other) noexcept		// Move constructor
	{
		id_ = other.id_;
		other.id_ = 0;  // set this to zero to prevent a delink when others destructor is called
	}
	RefLink(const RefLink<L>& other) noexcept	// Copy constructor
	{
		link(other.id_);
	}
	~RefLink()
	{
		delink();
	}
	bool operator<(const RefLink<L>& other) const
	{
		return id_ < other.id_;
	}
	RefLink<L>& operator=(RefLink<L>&& other) noexcept
	{
		delink();
		id_ = other.id_;
		other.id_ = 0;
		return *this;
	}
	RefLink& operator=(const RefLink& other) noexcept
	{
		delink();
		link(other.id_);
		return *this;
	}
	auto operator()() const
	{
		return id_;
	}
	bool linked() const
	{
		return id_ != 0;
	}
	void clear(){
		delink();
	}
	void unsafe_link(const size_type id){
		link(id);
	}
	void unsafe_clear()
	{
		id_ = 0;
	}
	static void setLinker(L* linker_)
	{
		linker = linker_;
	}
private:
	void link(const size_type id)
	{
		if (id != 0) {
			linker->link(id);
			id_ = id;
		}
	}
	void delink()
	{
		if (id_ != 0) {
			linker->delink(id_);
			id_ = 0;
		}
	}
	size_type id_ = 0;
	static L* linker;
};
template<typename L>
L* RefLink<L>::linker = nullptr;
}
