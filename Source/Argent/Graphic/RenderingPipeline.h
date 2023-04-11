#pragma once
#include <d3d12.h>
#include "Dx12/PipelineState.h"
#include "../Resource/ArShader.h"


namespace Argent
{
	namespace Graphics
	{
		class RenderingPipeline
		{
		public:
			/**
			 * \brief 
			 * \param vsFilePath 
			 * \param psFilePath 
			 * \param rootSigDesc 
			 * \param pipelineStateDesc 各種シェーダーとpRootSignatureは値をセットしなくてもいい
			 */
			RenderingPipeline(const char* vsFilePath, const char* psFilePath, 
			                   const  D3D12_ROOT_SIGNATURE_DESC* rootSigDesc,
			                    D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc);

			void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) const
			{
				pipelineState->SetOnCommandList(cmdList);
				rootSignature->SetOnCommandList(cmdList);
			}

			static std::unique_ptr<RenderingPipeline> CreateDefaultSpritePipeLine();
			static std::unique_ptr<RenderingPipeline> CreateDefaultStaticMeshPipeLine();
			static std::unique_ptr<RenderingPipeline> CreateDefaultSkinnedMeshPipeLine();
			static std::unique_ptr<RenderingPipeline> CreateDebugRendererPipeLine();

		private:
			/**
			 * \brief 頂点シェーダーとピクセルシェーダー、ルートシグネチャの値は入れなくてもいい
			 * \param device
			 * \param pipelineStateDesc 
			 */
			void CreatePipelineState(ID3D12Device* device, D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc)
			{
				if (rootSignature == nullptr) _ASSERT_EXPR(FALSE, "Call CreateRootSignature befor Calling CreatePipelineState");
				if (!vertexShader || !pixelShader) _ASSERT_EXPR(FALSE, "one or more shader are nullptr");

				pipelineStateDesc->pRootSignature = rootSignature->GetpRootSignature();
				pipelineStateDesc->VS.pShaderBytecode = vertexShader->GetData();
				pipelineStateDesc->VS.BytecodeLength = vertexShader->GetSize();
				pipelineStateDesc->PS.pShaderBytecode = pixelShader->GetData();
				pipelineStateDesc->PS.BytecodeLength = pixelShader->GetSize();
				pipelineState = std::make_shared<Dx12::PipelineState>(device, pipelineStateDesc);
			}
			void CreateRootSignature(ID3D12Device* device, const D3D12_ROOT_SIGNATURE_DESC* rootSigDesc)
			{
				rootSignature = std::make_shared<Dx12::RootSignature>(device, rootSigDesc);
			}

			std::shared_ptr<Dx12::PipelineState> pipelineState{};
			std::shared_ptr<Dx12::RootSignature> rootSignature{};
			std::shared_ptr<Shader::ArShader> vertexShader{};
			std::shared_ptr<Shader::ArShader> pixelShader{};
		};
		
	}
}

