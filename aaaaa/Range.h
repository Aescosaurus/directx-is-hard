#pragma once

#include "Random.h"
#include <algorithm>

// suck it windows
#undef min
#undef max

template<typename T>
class Range_
{
public:
	Range_( T min,T max )
		:
		min( min ),
		max( max )
	{
		if( min > max ) std::swap( min,max );
	}

	operator T() const
	{
		return( T( Random::Range( min,max ) ) );
	}
public:
	T min;
	T max;
};

typedef Range_<float> Range;
typedef Range_<int> RangeI;