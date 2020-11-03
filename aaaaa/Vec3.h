#pragma once

template<typename T>
class Vec3_
{
public:
	constexpr Vec3_()
		:
		Vec3_( T( 0.0 ),T( 0.0 ),T( 0.0 ) )
	{}
	constexpr Vec3_( T x,T y,T z )
		:
		x( x ),
		y( y ),
		z( z )
	{}

	template<typename U>
	constexpr operator Vec3_<U>()
	{
		return( Vec3_<U>{ U( x ),U( y ),U( z ) } );
	}

	Vec3_& operator+=( const Vec3_& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return( *this );
	}
	constexpr Vec3_ operator+( const Vec3_& rhs ) const
	{
		auto temp = *this;
		temp += rhs;
		return( temp );
	}
	Vec3_& operator-=( const Vec3_& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return( *this );
	}
	constexpr Vec3_ operator-( const Vec3_& rhs ) const
	{
		auto temp = *this;
		temp -= rhs;
		return( temp );
	}
	Vec3_& operator*=( float rhs )
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;

		return( *this );
	}
	constexpr Vec3_ operator*( T rhs ) const
	{
		auto temp = *this;
		temp *= rhs;
		return( temp );
	}
	Vec3_& operator/=( T rhs )
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;

		return( *this );
	}
	constexpr Vec3_ operator/( T rhs ) const
	{
		auto temp = *this;
		temp /= rhs;
		return( temp );
	}

	constexpr Vec3_& Dot( const Vec3_& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return( *this );
	}
	constexpr Vec3_ GetDot( const Vec3_& rhs ) const
	{
		auto temp = *this;

		temp.Dot( rhs );

		return( temp );
	}
	constexpr Vec3_& Cross( const Vec3_& rhs )
	{
		x = y * rhs.z - z * rhs.y;
		y = z * rhs.x - x * rhs.z;
		z = x * rhs.y - y * rhs.x;

		return( *this );
	}
	constexpr Vec3_ GetCross( const Vec3_& rhs ) const
	{
		auto temp = *this;

		temp.Cross( rhs );

		return( temp );
	}

	constexpr T GetLengthSq() const
	{
		return( x * x + y * y + z * z );
	}
	constexpr T GetLength() const
	{
		return( sqrt( GetLengthSq() ) );
	}

	constexpr Vec3_ Normalize()
	{
		const auto len = GetLength();
		if( len == T( 0.0 ) )
		{
			return( *this );
		}
		return( *this / len );
	}
	constexpr Vec3_ GetNormalized() const
	{
		auto temp = this;
		temp.Normalize();
		return( temp );
	}

	static constexpr Vec3_ Zero()
	{
		return( Vec3_{ T( 0.0 ),T( 0.0 ),T( 0.0 ) } );
	}
	static constexpr Vec3_ One()
	{
		return( Vec3_{ T( 1.0 ),T( 1.0 ),T( 1.0 ) } );
	}
public:
	T x;
	T y;
	T z;
};

typedef Vec3_<float> Vec3;
typedef Vec3_<int> Vei3;