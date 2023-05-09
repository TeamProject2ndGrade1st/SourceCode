#include "MeshRenderer.h"
#include "../GameObject/GameObject.h"
#include "../Other/Helper.h"
#include "../Resource/ResourceManager.h"
#include "../Graphic/Graphics.h"

namespace Argent::Component::Renderer
{
	MeshRenderer::MeshRenderer(ID3D12Device* device, const char* fileName,
		std::shared_ptr<Resource::Mesh::Mesh> meshes):
		BaseRenderer("Mesh Renderer")
	{
		this->mesh = meshes;
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
			const auto& material{ subset.material };
			//const auto& material{ materials.at(subset.materialUniqueId) };
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
		DirectX::XMFLOAT4X4 world{};
		DirectX::XMStoreFloat4x4(&world, GetOwner()->GetTransform()->CalcWorldMatrix());
		Render(Argent::Graphics::Graphics::Instance()->GetCommandList(Graphics::RenderType::Mesh), 
			//t->AdjustParentTransform().GetWorld());
			world);
	}

	void MeshRenderer::Update()
	{
		
	}


	void MeshRenderer::DrawDebug()
	{
		if (ImGui::TreeNode(GetName()))
		{
			if (ImGui::TreeNode("Material"))
			{
				for(auto& s : mesh->subsets)
				{
					s.material->DrawDebug();
				}
				ImGui::TreePop();
			}

			BaseRenderer::DrawDebug();
			ImGui::TreePop();
		}
	}

	void MeshRenderer::CreateComObject(ID3D12Device* device)
	{
		for(auto& s : mesh->subsets)
		{
			if (s.material->constantBuffer == nullptr)
			{
				s.material->constantBuffer = std::make_unique<Dx12::ArConstantBuffer<Material::MeshMaterial::Constant>>(
					device, Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor(),
					&s.material->constant);
			}
		}
		constantBuffer = std::make_unique<Argent::Dx12::ArConstantBuffer<Constants>>(device, Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor());
	}
}
