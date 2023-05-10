#pragma once
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
	class GaussianBlur
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texcoord;
		};
	public:
		GaussianBlur(ID3D12Device* device, D3D12_RESOURCE_DESC rsDesc, 
			float clearColor[4]);
		virtual ~GaussianBlur() = default;

		
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;	//レンダーターゲットorシェーダーリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> depthResource;	//深度バッファ
		std::unique_ptr<Resource::Shader> vertexShader;
		std::unique_ptr<Resource::Shader> pixelShader;
		std::unique_ptr<RenderingPipeline> renderingPipeline;
		std::shared_ptr<Dx12::ArVertexBuffer<Vertex>> vertexBuffer;
		Dx12::Descriptor* srvDescriptor;
		Dx12::Descriptor* rtvDescriptor;
		Dx12::Descriptor* dsvDescriptor;
		float clearColor[4];
	};
}

