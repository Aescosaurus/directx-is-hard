#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include "..\Common\DirectXHelper.h"

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include "Common\d3dx12.h"
#include <pix.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <vector>
#include <agile.h>
#include <concrt.h>

class Cube
{
public:
	Cube( const std::shared_ptr<DX::DeviceResources>& deviceResources )
		:
		m_deviceResources( deviceResources ),
		m_mappedConstantBuffer( nullptr ),
		m_radiansPerSecond( XM_PIDIV4 ),	// rotate 45 degrees per second
		m_angle( 0 )
	{
		ZeroMemory( &m_constantBufferData,sizeof( m_constantBufferData ) );

		CreateAsset();
	}
	~Cube()
	{
		m_constantBuffer->Unmap( 0,nullptr );
		m_mappedConstantBuffer = nullptr;
	}
	void Update( bool tracking,bool loadingComplete,const DX::StepTimer& timer )
	{
		if( !tracking )
		{
			// Rotate the cube a small amount.
			m_angle += static_cast< float >( timer.GetElapsedSeconds() ) * m_radiansPerSecond;

			Rotate( m_angle );
		}

		// Update the constant buffer resource.
		UINT8* destination = m_mappedConstantBuffer + ( m_deviceResources->GetCurrentFrameIndex() * c_alignedConstantBufferSize );
		memcpy( destination,&m_constantBufferData,sizeof( m_constantBufferData ) );
	}
	void Rotate( float angle )
	{
		XMStoreFloat4x4( &m_constantBufferData.model,XMMatrixTranspose( XMMatrixRotationY( angle ) ) );
	}
	void OnWindowResize()
	{
		// This sample makes use of a right-handed coordinate system using row-major matrices.
		XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
			fovAngleY,
			aspectRatio,
			0.01f,
			100.0f
		);

		XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
		XMMATRIX orientationMatrix = XMLoadFloat4x4( &orientation );

		XMStoreFloat4x4(
			&m_constantBufferData.projection,
			XMMatrixTranspose( perspectiveMatrix * orientationMatrix )
		);

		// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
		static const XMVECTORF32 eye = { 0.0f,0.7f,1.5f,0.0f };
		static const XMVECTORF32 at = { 0.0f,-0.1f,0.0f,0.0f };
		static const XMVECTORF32 up = { 0.0f,1.0f,0.0f,0.0f };

		XMStoreFloat4x4( &m_constantBufferData.view,XMMatrixTranspose( XMMatrixLookAtRH( eye,at,up ) ) );
	}
private:
	void CreateAsset()
	{
		// Cube vertices. Each vertex has a position and a color.
		VertexPositionColor cubeVertices[] =
		{
			// { XMFLOAT3( -0.5f,-0.5f,-0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) },
			// { XMFLOAT3( -0.5f,-0.5f,0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) },
			// { XMFLOAT3( -0.5f,0.5f,-0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) },
			// { XMFLOAT3( -0.5f,0.5f,0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) },
			// { XMFLOAT3( 0.5f,-0.5f,-0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) },
			// { XMFLOAT3( 0.5f,-0.5f,0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) },
			// { XMFLOAT3( 0.5f,0.5f,-0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) },
			// { XMFLOAT3( 0.5f,0.5f,0.5f ),XMFLOAT3( 1.0f,0.0f,0.0f ) }
			// 
			// { XMFLOAT3{ -0.5f,0.5f,0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 0
			// { XMFLOAT3{ 0.5f,0.5f,0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 1
			// { XMFLOAT3{ -0.5f,-0.5f,0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 2
			// { XMFLOAT3{ 0.5f,-0.5f,0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 3
			// { XMFLOAT3{ -0.5f,0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 4
			// { XMFLOAT3{ 0.5f,0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 5
			// { XMFLOAT3{ -0.5f,-0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 6
			// { XMFLOAT3{ 0.5f,-0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,0.0f } }, // 7
			// 
			{ XMFLOAT3{ -0.5f,0.5f,0.5f },XMFLOAT3{ 1.0f,0.0f,0.0f } }, // 0
			{ XMFLOAT3{ 0.5f,0.5f,0.5f },XMFLOAT3{ 1.0f,0.0f,0.0f } }, // 1
			{ XMFLOAT3{ -0.5f,-0.5f,0.5f },XMFLOAT3{ 1.0f,0.0f,0.0f } }, // 2
			{ XMFLOAT3{ 0.5f,-0.5f,0.5f },XMFLOAT3{ 1.0f,0.0f,0.0f } }, // 3
			// 
			{ XMFLOAT3{ 0.5f,0.5f,0.5f },XMFLOAT3{ 0.0f,1.0f,0.0f } }, // 1
			{ XMFLOAT3{ 0.5f,0.5f,-0.5f },XMFLOAT3{ 0.0f,1.0f,0.0f } }, // 5
			{ XMFLOAT3{ 0.5f,-0.5f,0.5f },XMFLOAT3{ 0.0f,1.0f,0.0f } }, // 3
			{ XMFLOAT3{ 0.5f,-0.5f,-0.5f },XMFLOAT3{ 0.0f,1.0f,0.0f } }, // 7
			// 
			{ XMFLOAT3{ 0.5f,0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,1.0f } }, // 5
			{ XMFLOAT3{ -0.5f,0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,1.0f } }, // 4
			{ XMFLOAT3{ 0.5f,-0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,1.0f } }, // 7
			{ XMFLOAT3{ -0.5f,-0.5f,-0.5f },XMFLOAT3{ 0.0f,0.0f,1.0f } }, // 6
			// 
			{ XMFLOAT3{ -0.5f,0.5f,-0.5f },XMFLOAT3{ 1.0f,1.0f,0.0f } }, // 4
			{ XMFLOAT3{ -0.5f,0.5f,0.5f },XMFLOAT3{ 1.0f,1.0f,0.0f } }, // 0
			{ XMFLOAT3{ -0.5f,-0.5f,-0.5f },XMFLOAT3{ 1.0f,1.0f,0.0f } }, // 6
			{ XMFLOAT3{ -0.5f,-0.5f,0.5f },XMFLOAT3{ 1.0f,1.0f,0.0f } }, // 2
			// 
			{ XMFLOAT3{ -0.5f,-0.5f,0.5f },XMFLOAT3{ 0.0f,1.0f,1.0f } }, // 2
			{ XMFLOAT3{ 0.5f,-0.5f,0.5f },XMFLOAT3{ 0.0f,1.0f,1.0f } }, // 3
			{ XMFLOAT3{ -0.5f,-0.5f,-0.5f },XMFLOAT3{ 0.0f,1.0f,1.0f } }, // 6
			{ XMFLOAT3{ 0.5f,-0.5f,-0.5f },XMFLOAT3{ 0.0f,1.0f,1.0f } }, // 7
			// 
			{ XMFLOAT3{ -0.5f,0.5f,-0.5f },XMFLOAT3{ 1.0f,0.0f,1.0f } }, // 4
			{ XMFLOAT3{ 0.5f,0.5f,-0.5f },XMFLOAT3{ 1.0f,0.0f,1.0f } }, // 5
			{ XMFLOAT3{ -0.5f,0.5f,0.5f },XMFLOAT3{ 1.0f,0.0f,1.0f } }, // 0
			{ XMFLOAT3{ 0.5f,0.5f,0.5f },XMFLOAT3{ 1.0f,0.0f,1.0f } } // 1
		};

		const UINT vertexBufferSize = sizeof( cubeVertices );

		// Create the vertex buffer resource in the GPU's default heap and copy vertex data into it using the upload heap.
		// The upload resource must not be released until after the GPU has finished using it.
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUpload;

		CD3DX12_HEAP_PROPERTIES defaultHeapProperties( D3D12_HEAP_TYPE_DEFAULT );
		CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer( vertexBufferSize );
		DX::ThrowIfFailed( d3dDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&vertexBufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS( &m_vertexBuffer ) ) );

		CD3DX12_HEAP_PROPERTIES uploadHeapProperties( D3D12_HEAP_TYPE_UPLOAD );
		DX::ThrowIfFailed( d3dDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&vertexBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &vertexBufferUpload ) ) );

		NAME_D3D12_OBJECT( m_vertexBuffer );

		// Upload the vertex buffer to the GPU.
		{
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = reinterpret_cast< BYTE* >( cubeVertices );
			vertexData.RowPitch = vertexBufferSize;
			vertexData.SlicePitch = vertexData.RowPitch;

			UpdateSubresources( m_commandList.Get(),m_vertexBuffer.Get(),vertexBufferUpload.Get(),0,0,1,&vertexData );

			CD3DX12_RESOURCE_BARRIER vertexBufferResourceBarrier =
				CD3DX12_RESOURCE_BARRIER::Transition( m_vertexBuffer.Get(),D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER );
			m_commandList->ResourceBarrier( 1,&vertexBufferResourceBarrier );
		}

		// Load mesh indices. Each trio of indices represents a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes 0, 2 and 1 from the vertex buffer compose the
		// first triangle of this mesh.
		unsigned short cubeIndices[] =
		{
			// 0,2,1, // -x
			// 1,2,3,
			// 
			// 4,5,6, // +x
			// 5,7,6,
			// 
			// 0,1,5, // -y
			// 0,5,4,
			// 
			// 2,6,7, // +y
			// 2,7,3,
			// 
			// 0,4,6, // -z
			// 0,6,2,
			// 
			// 1,3,7, // +z
			// 1,7,5,

			// +0
			0,3,2,
			0,1,3,
			// + 4
			4 + 0,4 + 3,4 + 2,
			4 + 0,4 + 1,4 + 3,
			// + 8
			8 + 0,8 + 3,8 + 2,
			8 + 0,8 + 1,8 + 3,
			// + 12
			12 + 0,12 + 3,12 + 2,
			12 + 0,12 + 1,12 + 3,
			// + 16
			16 + 0,16 + 3,16 + 2,
			16 + 0,16 + 1,16 + 3,
			// + 20
			20 + 0,20 + 3,20 + 2,
			20 + 0,20 + 1,20 + 3
		};

		const UINT indexBufferSize = sizeof( cubeIndices );

		// Create the index buffer resource in the GPU's default heap and copy index data into it using the upload heap.
		// The upload resource must not be released until after the GPU has finished using it.
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUpload;

		CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer( indexBufferSize );
		DX::ThrowIfFailed( d3dDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&indexBufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS( &m_indexBuffer ) ) );

		DX::ThrowIfFailed( d3dDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&indexBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &indexBufferUpload ) ) );

		NAME_D3D12_OBJECT( m_indexBuffer );

		// Upload the index buffer to the GPU.
		{
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = reinterpret_cast< BYTE* >( cubeIndices );
			indexData.RowPitch = indexBufferSize;
			indexData.SlicePitch = indexData.RowPitch;

			UpdateSubresources( m_commandList.Get(),m_indexBuffer.Get(),indexBufferUpload.Get(),0,0,1,&indexData );

			CD3DX12_RESOURCE_BARRIER indexBufferResourceBarrier =
				CD3DX12_RESOURCE_BARRIER::Transition( m_indexBuffer.Get(),D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_INDEX_BUFFER );
			m_commandList->ResourceBarrier( 1,&indexBufferResourceBarrier );
		}

		// Create a descriptor heap for the constant buffers.
		{
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			heapDesc.NumDescriptors = DX::c_frameCount;
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			// This flag indicates that this descriptor heap can be bound to the pipeline and that descriptors contained in it can be referenced by a root table.
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			DX::ThrowIfFailed( d3dDevice->CreateDescriptorHeap( &heapDesc,IID_PPV_ARGS( &m_cbvHeap ) ) );

			NAME_D3D12_OBJECT( m_cbvHeap );
		}

		CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer( DX::c_frameCount * c_alignedConstantBufferSize );
		DX::ThrowIfFailed( d3dDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&constantBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &m_constantBuffer ) ) );

		NAME_D3D12_OBJECT( m_constantBuffer );

		// Create constant buffer views to access the upload buffer.
		D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_constantBuffer->GetGPUVirtualAddress();
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle( m_cbvHeap->GetCPUDescriptorHandleForHeapStart() );
		m_cbvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		for( int n = 0; n < DX::c_frameCount; n++ )
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
			desc.BufferLocation = cbvGpuAddress;
			desc.SizeInBytes = c_alignedConstantBufferSize;
			d3dDevice->CreateConstantBufferView( &desc,cbvCpuHandle );

			cbvGpuAddress += desc.SizeInBytes;
			cbvCpuHandle.Offset( m_cbvDescriptorSize );
		}

		// Map the constant buffers.
		CD3DX12_RANGE readRange( 0,0 );		// We do not intend to read from this resource on the CPU.
		DX::ThrowIfFailed( m_constantBuffer->Map( 0,&readRange,reinterpret_cast< void** >( &m_mappedConstantBuffer ) ) );
		ZeroMemory( m_mappedConstantBuffer,DX::c_frameCount * c_alignedConstantBufferSize );
		// We don't unmap this until the app closes. Keeping things mapped for the lifetime of the resource is okay.

		// Close the command list and execute it to begin the vertex/index buffer copy into the GPU's default heap.
		DX::ThrowIfFailed( m_commandList->Close() );
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_deviceResources->GetCommandQueue()->ExecuteCommandLists( _countof( ppCommandLists ),ppCommandLists );

		// Create vertex/index buffer views.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof( VertexPositionColor );
		m_vertexBufferView.SizeInBytes = sizeof( cubeVertices );

		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.SizeInBytes = sizeof( cubeIndices );
		m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	}
private:
	static const UINT c_alignedConstantBufferSize = ( sizeof( ModelViewProjectionConstantBuffer ) + 255 ) & ~255;

	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;
	ModelViewProjectionConstantBuffer m_constantBufferData;
	UINT8* m_mappedConstantBuffer;
	UINT m_cbvDescriptorSize;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	float m_radiansPerSecond;
	float m_angle;
};