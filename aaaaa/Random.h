#pragma once

#include <random>
#include <cassert>

class Random
{
public:
	static float Range( float min,float max )
	{
		static std::mt19937 randGen{ std::random_device{}( ) };

		assert( min < max ); // Maybe this is better as an if.

		std::uniform_real_distribution<float> dist( min,max );
		return( dist( randGen ) );
	}
	// Never returns max.
	static int RangeI( int min,int max )
	{
		return( int( Range( float( min ),float( max ) ) ) );
	}
};