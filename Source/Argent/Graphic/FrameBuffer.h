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
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texcoord;
		};
	public:
		FrameBuffer(ID3D12Device* device, D3D12_RESOURCE_DESC rsDesc, 
			float clearColor[4]);
		virtual ~FrameBuffer() = default;


		void Begin(const Graphics* gfx) const;
		void End(const Graphics* gfx) const;
		void Draw(const Graphics* gfx) const;
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		std::unique_ptr<Argent::Resource::Shader> vertexShader;
		std::unique_ptr<Argent::Resource::Shader> pixelShader;
		std::unique_ptr<RenderingPipeline> renderingPipeline;
		std::shared_ptr<Argent::Dx12::ArVertexBuffer<Vertex>> vertexBuffer;
		Dx12::Descriptor* srvDescriptor;
		Dx12::Descriptor* rtvDescriptor;
		float clearColor[4];
	};
}
