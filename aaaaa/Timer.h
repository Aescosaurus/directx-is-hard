#pragma once

class Timer
{
public:
	Timer( float duration )
		:
		duration( duration )
	{}

	bool Update( float dt )
	{
		if( curTime <= duration ) curTime += dt;

		return( IsDone() );
	}

	void Reset()
	{
		curTime = 0.0f;
	}

	bool IsDone() const
	{
		return( curTime >= duration );
	}
public:
	float duration;
	float curTime = 0.0f;
};