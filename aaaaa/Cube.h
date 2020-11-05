#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include <d3d12.h>
#include "Vec3.h"
#include <DirectXMath.h>
#include "pch.h"

class Cube
{
public:
	enum class MatType
	{
		Identity,
		Scale,
		Rotation,
		Orbit,
		Translate,

		Size
	};
public:
	Cube( const Vec3& pos,const Vec3& scale,int xmMatSize = -1 )
		:
		pos( pos ),
		scale( scale ),
		matSize( xmMatSize )
	{
		TryAllocMat();
	}
	Cube( const Cube& rhs )
		:
		Cube( rhs.pos,rhs.scale,rhs.matSize )
	{
		*this = rhs;
	}
	Cube& operator=( const Cube& rhs )
	{
		pos = rhs.pos;
		scale = rhs.scale;
		matSize = rhs.matSize;
		TryAllocMat();

		return( *this );
	}
	~Cube()
	{
		FreeMats();
	}

	void TryAllocMat()
	{
		FreeMats();
		if( matSize > 0 )
		{
			for( int i = 0; i < int( MatType::Size ); ++i )
			{
				mats.emplace_back( ( int* )( malloc( matSize ) ) );
			}
		}
	}

	void FreeMats()
	{
		for( auto& mat : mats )
		{
			delete mat;
		}
		mats.clear();
	}

	bool IsOverlappingWith( const Cube& other ) const
	{
		return( Right() > other.Left() && Left() < other.Right() &&
			Bot() < other.Top() && Top() > other.Bot() &&
			Front() > other.Back() && Back() < other.Front() );
	}
private:
	float Left() const
	{
		return( pos.x - scale.x / 2.0f );
	}
	float Right() const
	{
		return( pos.x + scale.x / 2.0f );
	}
	float Top() const
	{
		return( pos.y + scale.y / 2.0f );
	}
	float Bot() const
	{
		return( pos.y - scale.y / 2.0f );
	}
	float Front() const
	{
		return( pos.z + scale.z / 2.0f );
	}
	float Back() const
	{
		return( pos.z - scale.z / 2.0f );
	}
public:
	// Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	// Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	// D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	// D3D12_INDEX_BUFFER_VIEW indexBufferView;
	
	Vec3 pos;
	Vec3 scale;
	// int* mat = nullptr;
	std::vector<int*> mats;
	int matSize;
};