#include "ArSkinnedMeshRenderer.h"
#include <filesystem>
#include "../Core/ArTimer.h"
#include "../GameObject/GameObject.h"
#include "../Other/ArHelper.h"
#include "../Resource/ArResourceManager.h"

namespace Argent::Component::Renderer
{
	ArSkinnedMeshRenderer::ArSkinnedMeshRenderer(ID3D12Device* device, const char* fileName,
		std::vector<std::shared_ptr<Resource::Mesh::ArSkinnedMesh>>& meshes,
		std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials, 
		std::vector<Resource::Animation::ArAnimation>& animation):
		ArRenderer("SkinnedMeshRenderer")
	{
		this->skinnedMeshes = meshes;
		for (auto& m : materials)
		{
			this->materials.emplace(m.first, std::move(m.second));
		}
		this->animationClips = animation;
		CreateComObject(device);
		CreateRootSignatureAndPipelineState();
	}

	void ArSkinnedMeshRenderer::Render(ID3D12GraphicsCommandList* cmdList, 
		const DirectX::XMFLOAT4X4& world,
		const Resource::Animation::ArAnimation::Keyframe* keyframe) const
	{
		ArRenderer::Render(cmdList);

		Argent::Graphics::ArGraphics::Instance()->SetSceneConstant(static_cast<UINT>(RootParameterIndex::cbScene));

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Constants constant{};
		constant.world = world;
		objectConstantBuffer->UpdateConstantBuffer(constant);

		objectConstantBuffer->SetOnCommandList(cmdList, static_cast<UINT>(RootParameterIndex::cbObject));
		for(const auto& m : skinnedMeshes)
		{
			for(const auto& s : m->subsets)
			{
				if (animationClips.size() > 0)
				{
					Argent::Resource::Mesh::ArSkinnedMesh::Constant meshConstant{};
					const size_t boneCount{ m->bindPose.bones.size() };
					for (int boneIndex = 0; boneIndex < boneCount; ++boneIndex)
					{
						const auto& bone{ m->bindPose.bones.at(boneIndex) };
						const Resource::Animation::ArAnimation::Keyframe::Node& boneNode{ keyframe->nodes.at(bone.nodeIndex) };
						DirectX::XMStoreFloat4x4(&meshConstant.boneTransforms[boneIndex],
							DirectX::XMLoadFloat4x4(&bone.offsetTransform) *
							DirectX::XMLoadFloat4x4(&boneNode.globalTransform) *
							DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&m->defaultGlobalTransform))
						);
					}
					const Resource::Animation::ArAnimation::Keyframe::Node meshNode{ keyframe->nodes.at(m->nodeIndex) };

					meshConstant.globalTransform = meshNode.globalTransform;
					m->constantBuffer->UpdateConstantBuffer(meshConstant);
				}

				const auto& material{ materials.at(s.materialUniqueId) };
				material.constantBuffer->UpdateConstantBuffer(material.constant);
				material.SetOnCommand(cmdList, static_cast<UINT>(RootParameterIndex::cbMaterial),
					static_cast<UINT>(RootParameterIndex::txAlbedo), 
					static_cast<UINT>(RootParameterIndex::txNormal));

				m->constantBuffer->SetOnCommandList(cmdList, static_cast<UINT>(RootParameterIndex::cbMesh));

				m->SetOnCommandList(cmdList);
				Resource::Mesh::ArSkinnedMesh::DrawCall(cmdList, s.indexCount, 1, s.startIndexLocation, 0, 0);
			}
		}
	}

	void ArSkinnedMeshRenderer::Render() const 
	{
		if(animationClips.size() > 0)
		{
			const Resource::Animation::ArAnimation& animation{ this->animationClips.at(clipIndex) };
			const Resource::Animation::ArAnimation::Keyframe& keyframe{ animation.sequence.at(static_cast<uint64_t>(frameIndex)) };

			//todo マテリアルの適用
			Render(Argent::Graphics::ArGraphics::Instance()->GetCommandList(), GetOwner()->GetTransform()->GetWorld(),
				 &keyframe);
		}
		else
		{
			Resource::Animation::ArAnimation::Keyframe key{};
			Render(Argent::Graphics::ArGraphics::Instance()->GetCommandList(), GetOwner()->GetTransform()->GetWorld(),
			 &key);
		}
	}

	void ArSkinnedMeshRenderer::Update()
	{
	#if 0
		static int clipIndex{};
		int frameIndex{};
		static float animationTick{};

		Animation& animation{ mesh->animationClips.at(clipIndex) };
		frameIndex = static_cast<int>(animationTick* animation.samplingRate);
		if(frameIndex > animation.sequence.size() - 1)
		{
			++clipIndex;
			if(clipIndex > mesh->animationClips.size() - 1)
			{
				clipIndex = 0;
			}
			frameIndex = 0;
			animationTick = 0;
		}
		else
		{
			animationTick += HighResolutionTimer::Instance().DeltaTime();
		}
		Animation::Keyframe& keyframe{ animation.sequence.at(frameIndex) };
		
	#if 1
		DirectX::XMStoreFloat4((&keyframe.nodes.at(0).rotation),
			DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), 1.5f));
		keyframe.nodes.at(0).translation.x = 0;
		mesh->UpdateAnimation(keyframe);
	#endif

		mesh->SetOnCommandList(ArGraphics::Instance()->GetCommandList(),
			meshTransform.GetWorld(ArGraphics::Instance()->CoordinateSystemTransforms[coordinateSystem], 
				ArGraphics::Instance()->scaleFactor),
			 meshColor.color, &keyframe);

	#endif

		if (animationClips.size() == 0) return;
		static float animationTick{};
		const Resource::Animation::ArAnimation& animation{ this->animationClips.at(clipIndex) };
		frameIndex = static_cast<float>(animationTick* animation.samplingRate);
		if(frameIndex > animation.sequence.size() - 1)
		{
			frameIndex = 0;

			animationTick = 0;
		}
		else
		{
			animationTick += Argent::Timer::ArTimer::Instance().DeltaTime();
		}
	}

#ifdef _DEBUG
	void ArSkinnedMeshRenderer::DrawDebug()
	{
		if (ImGui::TreeNode("Skinned Mesh Renderer"))
		{
			if (animationClips.size() > 0)
			{
				ImGui::SliderInt("Animation Clip", &clipIndex, 0, animationClips.size() - 1);
				ImGui::Text(animationClips.at(clipIndex).name.c_str());
			}

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

	void ArSkinnedMeshRenderer::CreateComObject(ID3D12Device* device)
	{
		for(auto it = materials.begin(); it != materials.end(); ++it)
		{
			it->second.constantBuffer = 
				std::make_unique<Dx12::ArConstantBuffer<Argent::Material::ArMeshMaterial::Constant>>(
					device, 
					Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor(),
					&it->second.constant);
		}
		objectConstantBuffer = std::make_unique<Dx12::ArConstantBuffer<Constants>>(device, Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor());
	}

	void ArSkinnedMeshRenderer::CreateRootSignatureAndPipelineState()
	{
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
			rootParam[static_cast<int>(RootParameterIndex::cbScene)] = Helper::Dx12::RootParameter::Generate(1, &range[0], D3D12_SHADER_VISIBILITY_ALL);
			rootParam[static_cast<int>(RootParameterIndex::cbObject)] = Helper::Dx12::RootParameter::Generate(1, &range[1], D3D12_SHADER_VISIBILITY_ALL);
			rootParam[static_cast<int>(RootParameterIndex::cbMesh)] = Helper::Dx12::RootParameter::Generate(1, &range[2], D3D12_SHADER_VISIBILITY_ALL);
			rootParam[static_cast<int>(RootParameterIndex::cbMaterial)] = Helper::Dx12::RootParameter::Generate(1, &range[3], D3D12_SHADER_VISIBILITY_ALL);
			rootParam[static_cast<int>(RootParameterIndex::txAlbedo)] = Helper::Dx12::RootParameter::Generate(1, &range[4], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParam[static_cast<int>(RootParameterIndex::txNormal)] = Helper::Dx12::RootParameter::Generate(1, &range[5], D3D12_SHADER_VISIBILITY_PIXEL);

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


			renderingPipeline = std::make_shared<Argent::Graphics::RenderingPipeline::ArBaseRenderingPipeline>(
				"./Resources/Shader/SkinnedMeshVertex.cso",
				"./Resources/Shader/SkinnedMeshPixel.cso",
				&rootSigDesc,
				&pipelineStateDesc
				);
		}
	}
}