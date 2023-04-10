#include "StaticMeshRenderer.h"
#include <filesystem>
#include "../Core/Timer.h"
#include "../GameObject/GameObject.h"
#include "../Other/Helper.h"
#include "../Resource/ResourceManager.h"
#include "../Graphic/Graphics.h"

namespace Argent::Component::Renderer
{
	StaticMeshRenderer::StaticMeshRenderer(ID3D12Device* device, const char* fileName,
		std::shared_ptr<Resource::Mesh::ArStaticMesh> meshes,
		std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials) :
		BaseRenderer("StaticMeshRenderer")
	{
		this->mesh = meshes;
		for (auto& m : materials)
		{
			this->materials.emplace(m.first, std::move(m.second));
		}
		CreateComObject(device);
		renderingPipeline = Graphics::RenderingPipeline::CreateDefaultStaticMeshPipeLine();
		//CreateRootSignatureAndPipelineState();
	}

	void StaticMeshRenderer::Render(ID3D12GraphicsCommandList* cmdList,
	                                  const DirectX::XMFLOAT4X4& world) const
	{
		BaseRenderer::Render(cmdList);
		Argent::Graphics::ArGraphics::Instance()->SetSceneConstant(0);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Constants constant{};
		constant.world = world;
		constantBuffer->UpdateConstantBuffer(constant);
		constantBuffer->SetOnCommandList(cmdList, 1);

		mesh->SetOnCommandList(cmdList);
		for (const Resource::Mesh::ArStaticMesh::Subset& subset : mesh->subsets)
		{
			const auto& material{ materials.at(subset.materialUniqueId) };
			Argent::Material::ArMeshMaterial::Constant tmpConstant;
			tmpConstant = material.constant;
			tmpConstant.color = material.color.GetColor();
			material.constantBuffer->UpdateConstantBuffer(tmpConstant);
			material.SetOnCommand(cmdList, 2,
				3, 4);
			cmdList->DrawIndexedInstanced(subset.indexCount, 1, subset.startIndexLocation, 0, 0);
		}
		
	}

	void StaticMeshRenderer::Initialize()
	{
		GameObject* g = GetOwner();
		g->GetTransform()->SetWorld(mesh->defaultGlobalTransform);
		g->SetName(mesh->GetName());
	}

	void StaticMeshRenderer::Render() const 
	{
		const Transform* t = GetOwner()->GetTransform();
		Render(Argent::Graphics::ArGraphics::Instance()->GetCommandList(), 
			t->AdjustParentTransform().GetWorld());
	}

	void StaticMeshRenderer::Update()
	{
		
	}

#ifdef _DEBUG
	void StaticMeshRenderer::DrawDebug()
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

			BaseRenderer::DrawDebug();
			ImGui::TreePop();
		}
	}
#endif
	void StaticMeshRenderer::CreateComObject(ID3D12Device* device)
	{
		for (auto it = materials.begin(); it != materials.end(); ++it)
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
