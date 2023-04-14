#include "SkinnedMeshRenderer.h"
#include "../Core/Timer.h"
#include "../GameObject/GameObject.h"
#include "../Other/Helper.h"
#include "../Resource/ResourceManager.h"
#include "../Graphic/Graphics.h"

namespace Argent::Component::Renderer
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(ID3D12Device* device, const char* fileName,
		std::shared_ptr<Resource::Mesh::ArSkinnedMesh> meshes,
		std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials,
		std::vector<Resource::Animation::AnimationClip>& animation) :
		BaseRenderer("SkinnedMesh Renderer")
	{
		this->skinnedMesh = meshes;
		for (auto& m : materials)
		{
			this->materials.emplace(m.first, std::move(m.second));
		}
		this->animationClips = animation;
		CreateComObject(device);
		renderingPipeline = Graphics::RenderingPipeline::CreateDefaultSkinnedMeshPipeLine();
	}


		

	void SkinnedMeshRenderer::Initialize()
	{
		GameObject* g = GetOwner();
		g->GetTransform()->SetWorld(skinnedMesh->localTransform);
		g->SetName(skinnedMesh->GetName());
	}

	void SkinnedMeshRenderer::Render(ID3D12GraphicsCommandList* cmdList, 
	                                   const DirectX::XMFLOAT4X4& world,
	                                   const Resource::Animation::AnimationClip::Keyframe* keyframe) const
	{
		BaseRenderer::Render(cmdList);
		Argent::Graphics::ArGraphics::Instance()->SetSceneConstant(static_cast<UINT>(RootParameterIndex::cbScene));

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Constants constant{};
		constant.world = world;
		objectConstantBuffer->UpdateConstantBuffer(constant);

		objectConstantBuffer->SetOnCommandList(cmdList, static_cast<UINT>(RootParameterIndex::cbObject));
		const auto& m = skinnedMesh;
		for(const auto& s : m->subsets)
		{
			if (animationClips.size() > 0)
			{
				Argent::Resource::Mesh::ArSkinnedMesh::Constant meshConstant{};
				const size_t boneCount{ m->bindPose.bones.size() };
				for (int boneIndex = 0; boneIndex < boneCount; ++boneIndex)
				{
					const auto& bone{ m->bindPose.bones.at(boneIndex) };
					const Resource::Animation::AnimationClip::Keyframe::Node& boneNode{ keyframe->nodes.at(bone.nodeIndex) };
					DirectX::XMStoreFloat4x4(&meshConstant.boneTransforms[boneIndex],
						DirectX::XMLoadFloat4x4(&bone.offsetTransform) *
						DirectX::XMLoadFloat4x4(&boneNode.globalTransform) *
						DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&m->localTransform))
					);
				}
				const Resource::Animation::AnimationClip::Keyframe::Node meshNode{ keyframe->nodes.at(m->nodeIndex) };

				meshConstant.globalTransform = meshNode.globalTransform;
				m->constantBuffer->UpdateConstantBuffer(meshConstant);
			}
			else
			{
				Argent::Resource::Mesh::ArSkinnedMesh::Constant meshConstant{};
				const size_t boneCount{ m->bindPose.bones.size() };
				for (int boneIndex = 0; boneIndex < boneCount; ++boneIndex)
				{
					const auto& bone{ m->bindPose.bones.at(boneIndex) };
					DirectX::XMStoreFloat4x4(&meshConstant.boneTransforms[boneIndex],
						DirectX::XMLoadFloat4x4(&bone.offsetTransform)  * DirectX::XMMatrixIdentity() * 
						DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixIdentity())
						//DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&m->localTransform))
					);
				}
				meshConstant.globalTransform = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
				m->constantBuffer->UpdateConstantBuffer(meshConstant);
			}

			const auto& material{ materials.at(s.materialUniqueId) };
			material.constantBuffer->UpdateConstantBuffer(material.constant);
			material.SetOnCommand(cmdList, static_cast<UINT>(RootParameterIndex::cbMaterial),
				static_cast<UINT>(RootParameterIndex::txAlbedo), 
				static_cast<UINT>(RootParameterIndex::txNormal));

			m->constantBuffer->SetOnCommandList(cmdList, static_cast<UINT>(RootParameterIndex::cbMesh));

			m->SetOnCommandList(cmdList);
			cmdList->DrawIndexedInstanced(s.indexCount, 1, s.startIndexLocation, 0, 0);
		}
	}

	void SkinnedMeshRenderer::Render() const 
	{
		Transform t = GetOwner()->GetTransform()->AdjustParentTransform();
		if(animationClips.size() > 0)
		{
			const Resource::Animation::AnimationClip& animation{ this->animationClips.at(clipIndex) };
			const Resource::Animation::AnimationClip::Keyframe& keyframe{ animation.sequence.at(static_cast<uint64_t>(frameIndex)) };

			//todo マテリアルの適用
			Render(Argent::Graphics::ArGraphics::Instance()->GetCommandList(), t.GetWorld(),
				 &keyframe);
		}
		else
		{
			Resource::Animation::AnimationClip::Keyframe key{};
			Render(Argent::Graphics::ArGraphics::Instance()->GetCommandList(), t.GetWorld(),
			 &key);
		}
	}

	void SkinnedMeshRenderer::Update()
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
		if (numUpdate == 1) _ASSERT_EXPR(FALSE, L"update call two times");
		++numUpdate;
		if (animationClips.size() == 0) return;
		//static float animationTick{};
		const Resource::Animation::AnimationClip& animation{ this->animationClips.at(clipIndex) };
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


	void SkinnedMeshRenderer::DrawDebug()
	{
		if (ImGui::TreeNode("Skinned Mesh Renderer"))
		{
			if (animationClips.size() > 0)
			{
				ImGui::SliderInt("Animation Clip", &clipIndex, 0, static_cast<int>(animationClips.size()) - 1);
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

			BaseRenderer::DrawDebug();
			ImGui::TreePop();
		}
	}


	void SkinnedMeshRenderer::CreateComObject(ID3D12Device* device)
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
}