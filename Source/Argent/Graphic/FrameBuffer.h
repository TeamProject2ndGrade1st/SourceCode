#pragma once
#include <d3d12.h>
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../Resource/Shader.h"
#include "Dx12/Descriptor.h"
#include "RenderingPipeline.h"
#include "Dx12/VertexBuffer.h"

namespace Argent::Graphics
{
	class Graphics;
}

namespace Argent::Graphics
{
	class FrameBuffer
	{
	public:
		FrameBuffer(ID3D12Device* device, D3D12_RESOURCE_DESC rsDesc,  DXGI_FORMAT format,
			float clearColor[4]);
		virtual ~FrameBuffer() = default;


		void Begin(const Graphics* gfx, ID3D12GraphicsCommandList* cList);
		void End(const Graphics* gfx) const;
		void Draw(const Graphics* gfx) const;

		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvGPUHandle()const { return srvDescriptor->GetGPUHandle(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;	//レンダーターゲットorシェーダーリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> depthResource;	//深度バッファ
		std::unique_ptr<RenderingPipeline> renderingPipeline;
		Dx12::Descriptor* srvDescriptor;
		Dx12::Descriptor* rtvDescriptor;
		Dx12::Descriptor* dsvDescriptor;
		float clearColor[4];
		ID3D12GraphicsCommandList* cmdList;
	};
}
