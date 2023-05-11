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
		struct BlurBuffer
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> blurResource[2];
			Dx12::Descriptor* srvDescriptor[2];
			Dx12::Descriptor* rtvDescriptor[2];
		};
		static constexpr int NumWeight = 8;
		struct Constant
		{
			float weight[NumWeight];
		};
		GaussianBlur(ID3D12Device* device, D3D12_RESOURCE_DESC rsDesc, 
			float clearColor[4]);
		virtual ~GaussianBlur() = default;

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, int rootParameterIndex);

		void Execute(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle);

		void UpdateWeight(float power);

		void Blurred(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect,
			BlurBuffer* buffer,
			D3D12_GPU_DESCRIPTOR_HANDLE srvHandle);
	private:
		
		std::unique_ptr<Argent::Dx12::ConstantBuffer<Constant>> constantBuffer;

		BlurBuffer blurResource[4];
		std::unique_ptr<RenderingPipeline> renderingPipelineX;
		std::unique_ptr<RenderingPipeline> renderingPipelineY;
		std::unique_ptr<RenderingPipeline> renderingPipelineFinal;

		//ç≈èIìIÇ»èoóÕäiî[óp
		Dx12::Descriptor* rtvFinal;
		Dx12::Descriptor* srvFinal;
		Microsoft::WRL::ComPtr<ID3D12Resource> finalResource;

		float clearColor[4];

		Constant constant;
	};
}

