#include "Material.h"
#include "ResourceManager.h"


namespace Argent::Material
{
	ArMaterial::ArMaterial(const char* textureFilepath):
		color(DirectX::XMFLOAT4(1, 1, 1, 1))
	{
		if(textureFilepath[0] != NULL)
		{
			textures.emplace_back(std::make_unique<Texture::ArTexture>(textureFilepath));
		}
	}

	void ArMaterial::Render(ID3D12GraphicsCommandList* cmdList, UINT RootParameterIndex) const
	{
		textures.at(0)->Render(cmdList, RootParameterIndex);
	}

	void ArMeshMaterial::CreateTexture(const char* filePath, TextureType type)
	{
		textureUniqueId[static_cast<UINT>(type)] = Argent::Resource::ArResourceManager::Instance().LoadTexture(filePath);
		textureNames[static_cast<UINT>(type)] = filePath;
	}

	void ArMeshMaterial::SetOnCommand(ID3D12GraphicsCommandList* cmdList, UINT cbIndex, UINT diffuseIndex,
		UINT normalIndex) const
	{
		//todo ‚È‚ñ‚Æ‚©”’
		constantBuffer->SetOnCommandList(cmdList, cbIndex);
		auto t1 = Resource::ArResourceManager::Instance().GetTexture(textureUniqueId[static_cast<int>(TextureType::Diffuse)]);
		auto t2 = Resource::ArResourceManager::Instance().GetTexture(textureUniqueId[static_cast<int>(TextureType::Normal)]);
		t1->Render(cmdList, diffuseIndex);
		t2->Render(cmdList, normalIndex);
	}

#ifdef _DEBUG
	void ArMaterial::DrawDebug()
	{
		if(ImGui::TreeNode("Material"))
		{
			ImGui::Text("%f", textures.at(0)->GetWidth());
			ImGui::Text("%f", textures.at(0)->GetHeight());
			ImGui::Image(reinterpret_cast<ImTextureID>(textures.at(0)->GetImDescriptor()->GetGPUHandle().ptr), ImVec2(128, 128));
			color.DrawDebug();
			ImGui::TreePop();
		}
	}
#endif



}
