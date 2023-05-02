#include "Material.h"
#include "ResourceManager.h"


namespace Argent::Material
{
	SpriteMaterial::SpriteMaterial(const char* textureFilepath):
		color(DirectX::XMFLOAT4(1, 1, 1, 1))
	{
		texture =std::make_unique<Resource::Texture>(textureFilepath);
	}

	void SpriteMaterial::Render(ID3D12GraphicsCommandList* cmdList, UINT RootParameterIndex) const
	{
		texture->SetOnCommandList(cmdList, RootParameterIndex);
	}

	void MeshMaterial::CreateTexture(const char* filePath, TextureUsage type)
	{
		textureUniqueId[static_cast<UINT>(type)] = Argent::Resource::ResourceManager::Instance().LoadTexture(filePath);
		textureNames[static_cast<UINT>(type)] = filePath;
	}

	void MeshMaterial::SetOnCommand(ID3D12GraphicsCommandList* cmdList, UINT cbIndex, UINT diffuseIndex,
		UINT normalIndex) const
	{
		//todo なんとか白
		constantBuffer->SetOnCommandList(cmdList, cbIndex);
		auto t1 = Resource::ResourceManager::Instance().GetTexture(textureUniqueId[static_cast<int>(TextureUsage::Diffuse)]);
		auto t2 = Resource::ResourceManager::Instance().GetTexture(textureUniqueId[static_cast<int>(TextureUsage::Normal)]);
		t1->SetOnCommandList(cmdList, diffuseIndex);
		t2->SetOnCommandList(cmdList, normalIndex);
	}


	void SpriteMaterial::DrawDebug()
	{
		if(ImGui::TreeNode("Material"))
		{
			ImGui::Text("%f", texture->GetWidth());
			ImGui::Text("%f", texture->GetHeight());
			ImGui::Image(reinterpret_cast<ImTextureID>(texture->GetImDescriptor()->GetGPUHandle().ptr), ImVec2(128, 128));
			color.DrawDebug();
			ImGui::TreePop();
		}
	}




}
