#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include <d3d12.h>
#include "Vec3.h"

class Cube
{
public:
	Cube( const Vec3& pos,const Vec3& scale )
		:
		pos( pos ),
		scale( scale )
	{}

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
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	
	Vec3 pos;
	Vec3 scale;
};