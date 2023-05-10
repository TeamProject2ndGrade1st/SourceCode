#pragma once
#pragma once
#include <d3d12.h>
#include <memory>
#include <wrl.h>
#include "Dx12/Descriptor.h"
#include "RenderingPipeline.h"

#include "Dx12/ConstantBuffer.h"


namespace Argent::Graphics
{
	class GaussianBlur
	{
	public:
		static constexpr int NumWeight = 8;
		struct Constant
		{
			float weight[NumWeight];
		};
		GaussianBlur(ID3D12Device* device, D3D12_RESOURCE_DESC rsDesc, 
			float clearColor[4]);
		virtual ~GaussianBlur() = default;

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, int rootParameterIndex);

		void Execute(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect);

		void UpdateWeight(float power);
	private:
		
		Microsoft::WRL::ComPtr<ID3D12Resource> blurResourceX;	//レンダーターゲットorシェーダーリソースMicrosoft::WRL::ComPtr<ID3D12Resource> depthResource;	//深度バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> finalBlurResource;	//レンダーターゲットorシェーダーリソースMicrosoft::WRL::ComPtr<ID3D12Resource> depthResource;	//深度バッファ
		std::unique_ptr<Argent::Dx12::ConstantBuffer<Constant>> constantBuffer;

		Dx12::Descriptor* srvDescriptorX;
		Dx12::Descriptor* rtvDescriptorX;
		Dx12::Descriptor* srvDescriptorFinal;
		Dx12::Descriptor* rtvDescriptorFinal;
		std::unique_ptr<RenderingPipeline> renderingPipelineX;
		std::unique_ptr<RenderingPipeline> renderingPipelineY;
		float clearColor[4];

		Constant constant;
	};
}

