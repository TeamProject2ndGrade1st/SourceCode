#include "RenderingPipeline.h"
#include "Graphics.h"

namespace Argent::Graphics
{
	RenderingPipeline::RenderingPipeline(const char* vsFilePath, const char* psFilePath,
		const D3D12_ROOT_SIGNATURE_DESC* rootSigDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc):
		vertexShader(std::make_shared<Shader::ArShader>(vsFilePath))
	,	pixelShader(std::make_shared<Shader::ArShader>(psFilePath))
	{
		ID3D12Device* device = Graphics::ArGraphics::Instance()->GetDevice();
		CreateRootSignature(device, rootSigDesc);
		CreatePipelineState(device, pipelineStateDesc);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDefaultSpritePipeLine()
	{
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};

		D3D12_DESCRIPTOR_RANGE range[1]{};

		range[0] = Helper::Dx12::DescriptorRange::Generate(0, 1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);

		D3D12_ROOT_PARAMETER rootParam[1]{};
		rootParam[0] = Helper::Dx12::RootParameter::Generate(1, &range[0], D3D12_SHADER_VISIBILITY_PIXEL);

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fAnisotropic, Helper::Dx12::Sampler::WrapMode::wBorder);


		rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSigDesc.NumParameters = 1;
		rootSigDesc.pParameters = rootParam;
		rootSigDesc.NumStaticSamplers = 1;
		rootSigDesc.pStaticSamplers = &samplerDesc;


		//パイプライン

		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0] = Helper::Dx12::Blend::GenerateRenderTargetBlendDesc(Helper::Dx12::Blend::BlendMode::bAlpha);

		D3D12_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT)
		};

		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;


		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		D3D12_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.DepthClipEnable = FALSE;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

		pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		pipelineStateDesc.RasterizerState = rasterizerDesc;
		pipelineStateDesc.BlendState = blendDesc;
		pipelineStateDesc.InputLayout.NumElements = _countof(inputElementDesc);
		pipelineStateDesc.InputLayout.pInputElementDescs = inputElementDesc;
		pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = 1;
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.SampleDesc = sampleDesc;
		pipelineStateDesc.DepthStencilState = depthStencilDesc;
		pipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		//todo shader filepath どっかにおいておきたいね
		return std::make_unique<RenderingPipeline>(
			"Resources/Shader/SpriteVertex.cso",
			"Resources/Shader/SpritePixel.cso",
			&rootSigDesc,
			&pipelineStateDesc);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDefaultStaticMeshPipeLine()
	{
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
		D3D12_DESCRIPTOR_RANGE range[5]{};

		range[0] = Helper::Dx12::DescriptorRange::Generate(0, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[1] = Helper::Dx12::DescriptorRange::Generate(1, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[2] = Helper::Dx12::DescriptorRange::Generate(2, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[3] = Helper::Dx12::DescriptorRange::Generate(0, 1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
		range[4] = Helper::Dx12::DescriptorRange::Generate(1, 1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);

		D3D12_ROOT_PARAMETER rootParam[5]{};
		rootParam[0] = Helper::Dx12::RootParameter::Generate(1, &range[0], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[1] = Helper::Dx12::RootParameter::Generate(1, &range[1], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[2] = Helper::Dx12::RootParameter::Generate(1, &range[2], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[3] = Helper::Dx12::RootParameter::Generate(1, &range[3], D3D12_SHADER_VISIBILITY_PIXEL);
		rootParam[4] = Helper::Dx12::RootParameter::Generate(1, &range[4], D3D12_SHADER_VISIBILITY_PIXEL);

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fPoint, Helper::Dx12::Sampler::WrapMode::wRepeat);


		rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSigDesc.NumParameters = 5;
		rootSigDesc.pParameters = rootParam;
		rootSigDesc.NumStaticSamplers = 1;
		rootSigDesc.pStaticSamplers = &samplerDesc;


		//パイプライン

		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0] = Helper::Dx12::Blend::GenerateRenderTargetBlendDesc(Helper::Dx12::Blend::BlendMode::bAlpha);

		D3D12_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
		};

		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;


		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


		pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		pipelineStateDesc.RasterizerState = Helper::Dx12::Rasterizer::Generate();
		pipelineStateDesc.BlendState = blendDesc;
		pipelineStateDesc.InputLayout.NumElements = _countof(inputElementDesc);
		pipelineStateDesc.InputLayout.pInputElementDescs = inputElementDesc;
		pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = 1;
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.SampleDesc = sampleDesc;
		pipelineStateDesc.DepthStencilState = depthStencilDesc;
		pipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


		return std::make_unique<Argent::Graphics::RenderingPipeline>(
			"./Resources/Shader/StaticMeshVertex.cso",
			"./Resources/Shader/StaticMeshPixel.cso",
			&rootSigDesc,
			&pipelineStateDesc
			);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDefaultSkinnedMeshPipeLine()
	{
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
		D3D12_DESCRIPTOR_RANGE range[6]{};

		range[0] = Helper::Dx12::DescriptorRange::Generate(0, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[1] = Helper::Dx12::DescriptorRange::Generate(1, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[2] = Helper::Dx12::DescriptorRange::Generate(2, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[3] = Helper::Dx12::DescriptorRange::Generate(3, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[4] = Helper::Dx12::DescriptorRange::Generate(0, 1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
		range[5] = Helper::Dx12::DescriptorRange::Generate(1, 1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);

		D3D12_ROOT_PARAMETER rootParam[6]{};
		rootParam[0] = Helper::Dx12::RootParameter::Generate(1, &range[0], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[1] = Helper::Dx12::RootParameter::Generate(1, &range[1], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[2] = Helper::Dx12::RootParameter::Generate(1, &range[2], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[3] = Helper::Dx12::RootParameter::Generate(1, &range[3], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[4] = Helper::Dx12::RootParameter::Generate(1, &range[4], D3D12_SHADER_VISIBILITY_PIXEL);
		rootParam[5] = Helper::Dx12::RootParameter::Generate(1, &range[5], D3D12_SHADER_VISIBILITY_PIXEL);

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fPoint, Helper::Dx12::Sampler::WrapMode::wRepeat);


		rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSigDesc.NumParameters = 6;
		rootSigDesc.pParameters = rootParam;
		rootSigDesc.NumStaticSamplers = 1;
		rootSigDesc.pStaticSamplers = &samplerDesc;


		//パイプライン

		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0] = Helper::Dx12::Blend::GenerateRenderTargetBlendDesc(Helper::Dx12::Blend::BlendMode::bAlpha);

		D3D12_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("WEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 1),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("BONES", DXGI_FORMAT_R32G32B32A32_UINT, 0, 1),
		};

		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;


		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


		pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		pipelineStateDesc.RasterizerState = Helper::Dx12::Rasterizer::Generate();
		pipelineStateDesc.BlendState = blendDesc;
		pipelineStateDesc.InputLayout.NumElements = _countof(inputElementDesc);
		pipelineStateDesc.InputLayout.pInputElementDescs = inputElementDesc;
		pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = 1;
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.SampleDesc = sampleDesc;
		pipelineStateDesc.DepthStencilState = depthStencilDesc;
		pipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


		return std::make_unique<Argent::Graphics::RenderingPipeline>(
			"./Resources/Shader/SkinnedMeshVertex.cso",
			"./Resources/Shader/SkinnedMeshPixel.cso",
			&rootSigDesc,
			&pipelineStateDesc
			);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDebugRendererPipeLine()
	{
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
		D3D12_DESCRIPTOR_RANGE range[2]{};

		range[0] = Helper::Dx12::DescriptorRange::Generate(0, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		range[1] = Helper::Dx12::DescriptorRange::Generate(1, 1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);

		D3D12_ROOT_PARAMETER rootParam[2]{};
		rootParam[0] = Helper::Dx12::RootParameter::Generate(1, &range[0], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[1] = Helper::Dx12::RootParameter::Generate(1, &range[1], D3D12_SHADER_VISIBILITY_ALL);

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fPoint, Helper::Dx12::Sampler::WrapMode::wRepeat);
		rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSigDesc.NumParameters = 2;
		rootSigDesc.pParameters = rootParam;
		rootSigDesc.NumStaticSamplers = 1;
		rootSigDesc.pStaticSamplers = &samplerDesc;


		//パイプライン
		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0] = Helper::Dx12::Blend::GenerateRenderTargetBlendDesc(Helper::Dx12::Blend::BlendMode::bAlpha);

		D3D12_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),

		};

		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;


		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


		D3D12_RASTERIZER_DESC rsDesc{};
		rsDesc.MultisampleEnable = FALSE;
		rsDesc.FrontCounterClockwise = TRUE;
		rsDesc.CullMode = D3D12_CULL_MODE_BACK;
		rsDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
		rsDesc.DepthClipEnable = TRUE;


		pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		
		pipelineStateDesc.RasterizerState = rsDesc;
		pipelineStateDesc.BlendState = blendDesc;
		pipelineStateDesc.InputLayout.NumElements = _countof(inputElementDesc);
		pipelineStateDesc.InputLayout.pInputElementDescs = inputElementDesc;
		pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = 1;
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.SampleDesc = sampleDesc;
		pipelineStateDesc.DepthStencilState = depthStencilDesc;
		pipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


		return std::make_unique<Argent::Graphics::RenderingPipeline>(
			"./Resources/Shader/DebugMeshVertex.cso",
			"./Resources/Shader/DebugMeshPixel.cso",
			&rootSigDesc,
			&pipelineStateDesc
			);
	}
}
