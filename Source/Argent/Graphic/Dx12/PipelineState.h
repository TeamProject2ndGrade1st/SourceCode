#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "../../Other/Misc.h"
#include "../../Other/Helper.h"

namespace Argent
{
	namespace Dx12
	{
		class PipelineState
		{
		public:
			PipelineState(ID3D12Device* device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC* graphicsPipelineDesc)
			{
				HRESULT hr = device->CreateGraphicsPipelineState(graphicsPipelineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
				_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
			}
			virtual ~PipelineState() = default;
			PipelineState(const PipelineState&) = delete;
			PipelineState(const PipelineState&&) = delete;
			const PipelineState& operator=(const PipelineState&) = delete;
			const PipelineState& operator=(const PipelineState&&) = delete;

			void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) const
			{
				cmdList->SetPipelineState(pipelineState.Get());
			}
		private:
			Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		};

		class RootSignature
		{
		public:
			RootSignature(ID3D12Device* device, const D3D12_ROOT_SIGNATURE_DESC* rootSigDesc)
			{
				D3D12_ROOT_SIGNATURE_DESC desc{};

				D3D12_DESCRIPTOR_RANGE range[1]{};

				range[0] = Helper::Dx12::DescriptorRange::Generate(0, 1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
																																	 
				D3D12_ROOT_PARAMETER rootParam[1]{};
				rootParam[0] = Helper::Dx12::RootParameter::Generate(1, &range[0], D3D12_SHADER_VISIBILITY_PIXEL);
																																  
				D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fAnisotropic, Helper::Dx12::Sampler::WrapMode::wBorder);


				desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
				desc.NumParameters = 1;
				desc.pParameters = rootParam;
				desc.NumStaticSamplers = 1;
				desc.pStaticSamplers = &samplerDesc;


				Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
				Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
				HRESULT hr{ S_OK };

				hr = D3D12SerializeRootSignature(rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, 
					rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

				hr = device->CreateRootSignature(0, 
					rootSigBlob->GetBufferPointer(), 
					rootSigBlob->GetBufferSize(), 
					IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
				_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
			}

			void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) const
			{
				cmdList->SetGraphicsRootSignature(rootSignature.Get());
			}

			ID3D12RootSignature* GetpRootSignature() const { return rootSignature.Get(); }
		private:
			Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		};
	}
}
	