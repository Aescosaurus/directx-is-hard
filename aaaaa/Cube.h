#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include <d3d12.h>

class Cube
{
public:
	Cube()
	{
	}
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
};