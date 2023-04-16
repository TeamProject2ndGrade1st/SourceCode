#pragma once
#include <d3d12.h>
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include "../Resource/ArShader.h"
#include "Dx12/Descriptor.h"
#include "RenderingPipeline.h"
#include "Dx12/VertexBuffer.h"

namespace Argent::Graphics
{
	class ArGraphics;
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
		FrameBuffer(ID3D12Device* device, D3D12_RESOURCE_DESC rDesc, float clearColor[4],
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc);
		virtual ~FrameBuffer() = default;


		void Begin(const ArGraphics* gfx) const;
		void End(const ArGraphics* gfx) const;
		void Draw(const ArGraphics* gfx) const;
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		//Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeline;
		//Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		//Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		std::unique_ptr<Argent::Shader::ArShader> vertexShader;
		std::unique_ptr<Argent::Shader::ArShader> pixelShader;
		std::unique_ptr<RenderingPipeline> renderingPipeline;
		std::shared_ptr<Argent::Dx12::ArVertexBuffer<Vertex>> vertexBuffer;
		//D3D12_VERTEX_BUFFER_VIEW vertexView{};
		Dx12::Descriptor* srvDescriptor;
		Dx12::Descriptor* rtvDescriptor;
		float clearColor[4];
	};
}
