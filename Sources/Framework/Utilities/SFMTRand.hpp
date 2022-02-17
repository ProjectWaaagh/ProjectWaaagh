#ifndef PROJECTWAAAGH_SFMTRAND_HPP
#define PROJECTWAAAGH_SFMTRAND_HPP

#include "Defines.hpp"

#include <SFMT.h>
#include <new>


class SFMTRand {
public:
	SFMTRand();
	uint32 RandomUInt32();
	void* operator new(size_t size, std::nothrow_t const&);
	void operator delete(void* ptr, std::nothrow_t const&);

	void* operator new(size_t size);
	void operator delete(void* ptr);

	void* operator new[](size_t size, std::nothrow_t const&);
	void operator delete[](void* ptr, std::nothrow_t const&);

	void* operator new[](size_t size);
	void operator delete[](void* ptr);

private:
	sfmt_t mState;
};
#endif // !PROJECTWAAAGH_SFMTRAND_HPP
