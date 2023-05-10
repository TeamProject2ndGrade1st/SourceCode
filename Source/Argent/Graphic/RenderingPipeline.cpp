#include "RenderingPipeline.h"
#include "Graphics.h"

namespace Argent::Graphics
{
	RenderingPipeline::RenderingPipeline(const char* vsFilePath, const char* psFilePath,
		const D3D12_ROOT_SIGNATURE_DESC* rootSigDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc) :
		vertexShader(std::make_shared<Resource::Shader>(vsFilePath))
		, pixelShader(std::make_shared<Resource::Shader>(psFilePath))
	{
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();
		CreateRootSignature(device, rootSigDesc);
		CreatePipelineState(device, pipelineStateDesc);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDefaultSpritePipeline()
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

		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0] = rtvBlendDesc;

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
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
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
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		//pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.SampleDesc = sampleDesc;
		pipelineStateDesc.DepthStencilState = depthStencilDesc;
		pipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		pipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;



		//todo shader filepath どっかにおいておきたいね
		return std::make_unique<RenderingPipeline>(
			"Resources/Shader/SpriteVertex.cso",
			"Resources/Shader/SpritePixel.cso",
			&rootSigDesc,
			&pipelineStateDesc);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDefaultStaticMeshPipeline()
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
		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0] = rtvBlendDesc;
		//blendDesc.RenderTarget[0] = Helper::Dx12::Blend::GenerateRenderTargetBlendDesc(Helper::Dx12::Blend::BlendMode::bAlpha);



		D3D12_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TANGENT", DXGI_FORMAT_R32G32B32A32_FLOAT),
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
		};

		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;


		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;


		pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		auto r = Helper::Dx12::Rasterizer::Generate();
		//r.DepthClipEnable = false;

		pipelineStateDesc.RasterizerState = r;
		pipelineStateDesc.BlendState = blendDesc;
		pipelineStateDesc.InputLayout.NumElements = _countof(inputElementDesc);
		pipelineStateDesc.InputLayout.pInputElementDescs = inputElementDesc;
		pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = 1;
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
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

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDefaultSkinnedMeshPipeline()
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

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fAnisotropic, Helper::Dx12::Sampler::WrapMode::wRepeat);


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
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TANGENT", DXGI_FORMAT_R32G32B32A32_FLOAT),
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
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
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

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDebugRendererPipeline()
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
			Helper::Dx12::InputElement::GenerateInputLayoutDesc("TANGENT", DXGI_FORMAT_R32G32B32A32_FLOAT),
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
		rsDesc.CullMode = D3D12_CULL_MODE_NONE;
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
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
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

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateDefaultFrameBufferPipeline()
	{
		HRESULT hr{ S_OK };
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		//ルートシグネチャ用
		D3D12_DESCRIPTOR_RANGE range[1]{};
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].BaseShaderRegister = 0;
		range[0].NumDescriptors = 1;

		D3D12_ROOT_PARAMETER rootParam[1]{};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;

		D3D12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 1;
		rootSignatureDesc.pParameters = rootParam;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		D3D12_INPUT_ELEMENT_DESC inElementDesc[]
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = true;
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


		//パイプラインステート用
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

#if 0
		pipelineDesc.InputLayout.NumElements = _countof(inElementDesc);
		pipelineDesc.InputLayout.pInputElementDescs = inElementDesc;
#else
		pipelineDesc.InputLayout.NumElements = 0;
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
#endif
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = FALSE;
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0] = rtvBlendDesc;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		//pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		pipelineDesc.SampleDesc.Count = 1;
		pipelineDesc.SampleDesc.Quality = 0;
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		std::string vertexShaderName;

#if 0
		vertexShaderName = "./Resources/Shader/PeraVertex.cso";
#else
		vertexShaderName = "./Resources/Shader/FullscreenQuadVertex.cso";
#endif
		return std::make_unique<RenderingPipeline>(
			vertexShaderName.c_str(),
			"./Resources/Shader/FullscreenQuadPixel.cso",
			&rootSignatureDesc, &pipelineDesc
		);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateLuminanceExtractionPipeline()
	{
		HRESULT hr{ S_OK };
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		//ルートシグネチャ用
		D3D12_DESCRIPTOR_RANGE range[1]{};
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].BaseShaderRegister = 0;
		range[0].NumDescriptors = 1;

		D3D12_ROOT_PARAMETER rootParam[1]{};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;

		D3D12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 1;
		rootSignatureDesc.pParameters = rootParam;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = true;
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


		//パイプラインステート用
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
		pipelineDesc.InputLayout.NumElements = 0;
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = FALSE;
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0] = rtvBlendDesc;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		pipelineDesc.SampleDesc.Count = 1;
		pipelineDesc.SampleDesc.Quality = 0;
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


		return std::make_unique<RenderingPipeline>(
			"./Resources/Shader/FullscreenQuadVertex.cso",
			"./Resources/Shader/LuminanceExtractionPixel.cso",
			&rootSignatureDesc, &pipelineDesc
		);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateGaussianBlurPipelineX()
	{
		HRESULT hr{ S_OK };
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		//ルートシグネチャ用
		D3D12_DESCRIPTOR_RANGE range[2]{};
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].BaseShaderRegister = 0;
		range[0].NumDescriptors = 1;
		range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		range[1].BaseShaderRegister = 0;
		range[1].NumDescriptors = 1;

		D3D12_ROOT_PARAMETER rootParam[2]{};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[1].DescriptorTable.pDescriptorRanges = &range[1];
		rootParam[1].DescriptorTable.NumDescriptorRanges = 1;

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fAnisotropic, Helper::Dx12::Sampler::WrapMode::wBorder);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pParameters = rootParam;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = true;
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


		//パイプラインステート用
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
		pipelineDesc.InputLayout.NumElements = 0;
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = FALSE;
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0] = rtvBlendDesc;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		pipelineDesc.SampleDesc.Count = 1;
		pipelineDesc.SampleDesc.Quality = 0;
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


		return std::make_unique<RenderingPipeline>(
			"./Resources/Shader/GaussianBlurYVertex.cso",
			"./Resources/Shader/GaussianBlurPixel.cso",
			&rootSignatureDesc, &pipelineDesc
		);
	}
	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateGaussianBlurPipelineY()
	{
		HRESULT hr{ S_OK };
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		//ルートシグネチャ用
		D3D12_DESCRIPTOR_RANGE range[2]{};
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].BaseShaderRegister = 0;
		range[0].NumDescriptors = 1;
		range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		range[1].BaseShaderRegister = 0;
		range[1].NumDescriptors = 1;

		D3D12_ROOT_PARAMETER rootParam[2]{};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[1].DescriptorTable.pDescriptorRanges = &range[1];
		rootParam[1].DescriptorTable.NumDescriptorRanges = 1;

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fAnisotropic, Helper::Dx12::Sampler::WrapMode::wBorder);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pParameters = rootParam;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = true;
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


		//パイプラインステート用
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
		pipelineDesc.InputLayout.NumElements = 0;
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = FALSE;
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0] = rtvBlendDesc;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		pipelineDesc.SampleDesc.Count = 1;
		pipelineDesc.SampleDesc.Quality = 0;
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


		return std::make_unique<RenderingPipeline>(
			"./Resources/Shader/GaussianBlurYVertex.cso",
			"./Resources/Shader/GaussianBlurPixel.cso",
			&rootSignatureDesc, &pipelineDesc
		);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateTmpPipeline()
	{
				HRESULT hr{ S_OK };
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		//ルートシグネチャ用
		D3D12_DESCRIPTOR_RANGE range[2]{};
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].BaseShaderRegister = 0;
		range[0].NumDescriptors = 1;
		range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[1].BaseShaderRegister = 1;
		range[1].NumDescriptors = 1;

		D3D12_ROOT_PARAMETER rootParam[2]{};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[1].DescriptorTable.pDescriptorRanges = &range[1];
		rootParam[1].DescriptorTable.NumDescriptorRanges = 1;

		D3D12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pParameters = rootParam;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = true;
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


		//パイプラインステート用
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

#if 0
		pipelineDesc.InputLayout.NumElements = _countof(inElementDesc);
		pipelineDesc.InputLayout.pInputElementDescs = inElementDesc;
#else
		pipelineDesc.InputLayout.NumElements = 0;
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
#endif
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = FALSE;
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0] = rtvBlendDesc;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		//pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		pipelineDesc.SampleDesc.Count = 1;
		pipelineDesc.SampleDesc.Quality = 0;
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		std::string vertexShaderName;

#if 0
		vertexShaderName = "./Resources/Shader/PeraVertex.cso";
#else
		vertexShaderName = "./Resources/Shader/FullscreenQuadVertex.cso";
#endif
		return std::make_unique<RenderingPipeline>(
			vertexShaderName.c_str(),
			"./Resources/Shader/tmpPixel.cso",
			&rootSignatureDesc, &pipelineDesc
		);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateBloomPipeline()
	{
		HRESULT hr{ S_OK };
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		//ルートシグネチャ用
		D3D12_DESCRIPTOR_RANGE range[1]{};
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].BaseShaderRegister = 0;
		range[0].NumDescriptors = 1;


		D3D12_ROOT_PARAMETER rootParam[1]{};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;


		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fAnisotropic, Helper::Dx12::Sampler::WrapMode::wBorder);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 1;
		rootSignatureDesc.pParameters = rootParam;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc = Helper::Dx12::Blend::GenerateRenderTargetBlendDesc(Helper::Dx12::Blend::BlendMode::bAdd);

		//パイプラインステート用
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

#if 0
		pipelineDesc.InputLayout.NumElements = _countof(inElementDesc);
		pipelineDesc.InputLayout.pInputElementDescs = inElementDesc;
#else
		pipelineDesc.InputLayout.NumElements = 0;
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
#endif
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = FALSE;
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0] = rtvBlendDesc;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		//pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		pipelineDesc.SampleDesc.Count = 1;
		pipelineDesc.SampleDesc.Quality = 0;
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		std::string vertexShaderName;

#if 0
		vertexShaderName = "./Resources/Shader/PeraVertex.cso";
#else
		vertexShaderName = "./Resources/Shader/FullscreenQuadVertex.cso";
#endif
		return std::make_unique<RenderingPipeline>(
			vertexShaderName.c_str(),
			"./Resources/Shader/FullscreenQuadPixel.cso",
			&rootSignatureDesc, &pipelineDesc
		);
	}

	std::unique_ptr<RenderingPipeline> RenderingPipeline::CreateCombineBokeColorPipeline()
	{
		HRESULT hr{ S_OK };
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		//ルートシグネチャ用
		D3D12_DESCRIPTOR_RANGE range[4]{};
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].BaseShaderRegister = 0;
		range[0].NumDescriptors = 1;
		range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[1].BaseShaderRegister = 1;
		range[1].NumDescriptors = 1;
		range[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[2].BaseShaderRegister = 2;
		range[2].NumDescriptors = 1;
		range[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[3].BaseShaderRegister = 3;
		range[3].NumDescriptors = 1;

		D3D12_ROOT_PARAMETER rootParam[4]{};
		rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[0].DescriptorTable.pDescriptorRanges = &range[0];
		rootParam[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[1].DescriptorTable.pDescriptorRanges = &range[1];
		rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
		rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[2].DescriptorTable.pDescriptorRanges = &range[2];
		rootParam[2].DescriptorTable.NumDescriptorRanges = 1;
		rootParam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam[3].DescriptorTable.pDescriptorRanges = &range[3];
		rootParam[3].DescriptorTable.NumDescriptorRanges = 1;

		D3D12_STATIC_SAMPLER_DESC samplerDesc = Helper::Dx12::Sampler::GenerateSamplerDesc(Helper::Dx12::Sampler::FilterMode::fPoint, Helper::Dx12::Sampler::WrapMode::wBorder);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 4;
		rootSignatureDesc.pParameters = rootParam;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		D3D12_RENDER_TARGET_BLEND_DESC rtvBlendDesc{};
		rtvBlendDesc.BlendEnable = true;
		rtvBlendDesc.BlendEnable = TRUE;
		rtvBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtvBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rtvBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		rtvBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		rtvBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		rtvBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		rtvBlendDesc.LogicOpEnable = FALSE;
		rtvBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


		//パイプラインステート用
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
		pipelineDesc.InputLayout.NumElements = 0;
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = FALSE;
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0] = rtvBlendDesc;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		pipelineDesc.SampleDesc.Count = 1;
		pipelineDesc.SampleDesc.Quality = 0;
		pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


		return std::make_unique<RenderingPipeline>(
			"./Resources/Shader/GaussianBlurYVertex.cso",
			"./Resources/Shader/KawaseBloomPixel.cso",
			&rootSignatureDesc, &pipelineDesc
			);
	}
}
