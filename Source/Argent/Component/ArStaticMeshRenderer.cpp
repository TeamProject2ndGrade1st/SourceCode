#include "ArStaticMeshRenderer.h"
#include <filesystem>
#include "../Core/ArTimer.h"
#include "../GameObject/GameObject.h"
#include "../Other/ArHelper.h"
#include "../Resource/ArResourceManager.h"

namespace Argent::Component::Renderer
{
	ArStaticMeshRenderer::ArStaticMeshRenderer(ID3D12Device* device, const char* fileName, std::vector<std::shared_ptr<Resource::Mesh::ArStaticMesh>> meshes,
		std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials):
		ArRenderer("StaticMeshRenderer")
	{
		this->meshes = meshes;
		for(auto& m : materials)
		{
			this->materials.emplace(m.first, std::move(m.second));
		}
		CreateComObject(device);
		CreateRootSignatureAndPipelineState();
	}

	void ArStaticMeshRenderer::Render(ID3D12GraphicsCommandList* cmdList,
		const DirectX::XMFLOAT4X4& world) const
	{
		ArRenderer::Render(cmdList);
		Argent::Graphics::ArGraphics::Instance()->SetSceneConstant(static_cast<UINT>(RootParameterIndex::cbScene));

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Constants constant{};
		constant.world = world;
		constantBuffer->UpdateConstantBuffer(constant);
		for (const auto& mesh : meshes)
		{
			constantBuffer->SetOnCommandList(cmdList, static_cast<UINT>(RootParameterIndex::cbObject));

			mesh->SetOnCommandList(cmdList);
			for (const Resource::Mesh::ArStaticMesh::Subset& subset : mesh->subsets)
			{
				const auto& material{ materials.at(subset.materialUniqueId) };
				Argent::Material::ArMeshMaterial::Constant tmpConstant;
				tmpConstant = material.constant;
				tmpConstant.color = material.color.GetColor();
				material.constantBuffer->UpdateConstantBuffer(tmpConstant);
				material.SetOnCommand(cmdList, static_cast<UINT>(RootParameterIndex::cbMaterial),
					static_cast<UINT>(RootParameterIndex::txAlbedo), static_cast<UINT>(RootParameterIndex::txNormal));
				cmdList->DrawIndexedInstanced(subset.indexCount, 1, subset.startIndexLocation, 0, 0);
			}
		}
	}

	void ArStaticMeshRenderer::CreateRootSignatureAndPipelineState()
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
		rootParam[static_cast<int>(RootParameterIndex::cbScene)] = Helper::Dx12::RootParameter::Generate(1, &range[0], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[static_cast<int>(RootParameterIndex::cbObject)] = Helper::Dx12::RootParameter::Generate(1, &range[1], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[static_cast<int>(RootParameterIndex::cbMaterial)] = Helper::Dx12::RootParameter::Generate(1, &range[2], D3D12_SHADER_VISIBILITY_ALL);
		rootParam[static_cast<int>(RootParameterIndex::txAlbedo)] = Helper::Dx12::RootParameter::Generate(1, &range[3], D3D12_SHADER_VISIBILITY_PIXEL);
		rootParam[static_cast<int>(RootParameterIndex::txNormal)] = Helper::Dx12::RootParameter::Generate(1, &range[4], D3D12_SHADER_VISIBILITY_PIXEL);

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


		renderingPipeline = std::make_shared<Argent::Graphics::RenderingPipeline::ArBaseRenderingPipeline>(
			"./Resources/Shader/StaticMeshVertex.cso",
			"./Resources/Shader/StaticMeshPixel.cso",
			&rootSigDesc,
			&pipelineStateDesc
			);
	}

	void ArStaticMeshRenderer::Render() const 
	{
		Render(Argent::Graphics::ArGraphics::Instance()->GetCommandList(), 
			GetOwner()->GetTransform()->GetWorld());
	}

	void ArStaticMeshRenderer::Update()
	{
		
	}

#ifdef _DEBUG
	void ArStaticMeshRenderer::DrawDebug()
	{
		if (ImGui::TreeNode(GetName().c_str()))
		{
			if (ImGui::TreeNode("Material"))
			{
				for (auto& m : materials)
				{
					m.second.DrawDebug();
				}
				ImGui::TreePop();
			}

			ArRenderer::DrawDebug();
			ImGui::TreePop();
		}
	}
#endif
	void ArStaticMeshRenderer::CreateComObject(ID3D12Device* device)
	{
		HRESULT hr{ S_OK };

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = static_cast<UINT>(materials.size());
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		for (auto it = materials.begin();
			it != materials.end(); ++it)
{
			it->second.constantBuffer =
				std::make_unique<Argent::Dx12::ArConstantBuffer<Material::ArMeshMaterial::Constant>>(
					device,
					Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor(),
					&it->second.constant);
		}
		constantBuffer = std::make_unique<Argent::Dx12::ArConstantBuffer<Constants>>(device, Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor());
	}
}
