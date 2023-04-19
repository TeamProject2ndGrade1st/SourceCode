#include "MeshRenderer.h"
#include "../GameObject/GameObject.h"
#include "../Other/Helper.h"
#include "../Resource/ResourceManager.h"
#include "../Graphic/Graphics.h"

namespace Argent::Component::Renderer
{
	MeshRenderer::MeshRenderer(ID3D12Device* device, const char* fileName,
		std::shared_ptr<Resource::Mesh::ArMesh> meshes,
		std::unordered_map<uint64_t, std::shared_ptr<Material::MeshMaterial>>& materials):
		BaseRenderer("Mesh Renderer")
	{
		this->mesh = meshes;
		for (auto& m : materials)
		{
			this->materials.emplace(m.first, std::move(m.second));
		}
		CreateComObject(device);
		renderingPipeline = Graphics::RenderingPipeline::CreateDefaultStaticMeshPipeline();
	}

	void MeshRenderer::Render(ID3D12GraphicsCommandList* cmdList,
	                                const DirectX::XMFLOAT4X4& world) const
	{
		BaseRenderer::Render(cmdList);
		Argent::Graphics::Graphics::Instance()->SetSceneConstant(0);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Constants constant{};
		constant.world = world;
		constantBuffer->UpdateConstantBuffer(constant);
		constantBuffer->SetOnCommandList(cmdList, 1);

		mesh->SetOnCommandList(cmdList);
		for (const auto& subset : mesh->subsets)
		{
			const auto& material{ materials.at(subset.materialUniqueId) };
			Argent::Material::MeshMaterial::Constant tmpConstant;
			tmpConstant = material->constant;
			tmpConstant.color = material->color.GetColor();
			material->constantBuffer->UpdateConstantBuffer(tmpConstant);
			material->SetOnCommand(cmdList, 2,
				3, 4);
			cmdList->DrawIndexedInstanced(subset.indexCount, 1, subset.startIndexLocation, 0, 0);
		}
	}

	void MeshRenderer::Initialize()
	{
		GameObject* g = GetOwner();
		g->GetTransform()->SetWorld(mesh->localTransform);
		if(g->GetParent())
			g->SetName(mesh->GetName());
	}

	void MeshRenderer::Render() const 
	{
		const Transform* t = GetOwner()->GetTransform();
		Render(Argent::Graphics::Graphics::Instance()->GetCommandList(Graphics::RenderType::Mesh), 
			t->AdjustParentTransform().GetWorld());
	}

	void MeshRenderer::Update()
	{
		
	}


	void MeshRenderer::DrawDebug()
	{
		if (ImGui::TreeNode(GetName().c_str()))
		{
			if (ImGui::TreeNode("Material"))
			{
				for (auto& m : materials)
				{
					m.second->DrawDebug();
				}
				ImGui::TreePop();
			}

			BaseRenderer::DrawDebug();
			ImGui::TreePop();
		}
	}

	void MeshRenderer::CreateComObject(ID3D12Device* device)
	{
		for (auto it = materials.begin(); it != materials.end(); ++it)
		{
			it->second->constantBuffer =
				std::make_unique<Argent::Dx12::ArConstantBuffer<Material::MeshMaterial::Constant>>(
					device,
					Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor(),
					&it->second->constant);
		}
		constantBuffer = std::make_unique<Argent::Dx12::ArConstantBuffer<Constants>>(device, Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor());
	}
}
