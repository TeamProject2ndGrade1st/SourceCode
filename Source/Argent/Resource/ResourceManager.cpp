#include "ResourceManager.h"
#include "../../../imgui/ImguiCtrl.h"
#include <fstream>



#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

namespace Argent::Resource
{
	void ResourceManager::Initialize()
	{
		Clear();
	}

	void ResourceManager::Clear()
	{
		//meshData.clear();
	}

	void ResourceManager::Serialize()
	{
		

		for(auto& res : fbxResources)
		{
			std::filesystem::path cerealFileName(res.filePath);
			cerealFileName.replace_extension("cereal");
			std::ofstream ofs(cerealFileName.c_str(), std::ios::binary);
			cereal::BinaryOutputArchive serialization(ofs);
			serialization(res);
		}
	}

	uint64_t ResourceManager::LoadTexture(const char* filePath)
	{
		std::shared_ptr<Resource::Texture> ret = FindResourceFromFilePath(filePath);
		if(ret) 
			return ret->GetUniqueId();

		ret = std::make_shared<Argent::Resource::Texture>(filePath);

		textures[ret->GetUniqueId()] = ret;

		resources.emplace_back(ret);

		return ret->GetUniqueId();
	}

	std::shared_ptr<Material::MeshMaterial> ResourceManager::GetMaterial(const char* name)
	{
		for(auto& m : materials)
		{
			if(m.second.lock()->CompareName(name) && !m.second.expired())
			{
				return m.second.lock();
			}
		}
		return nullptr;
	}

	std::shared_ptr<Mesh::Mesh> ResourceManager::GetMesh(const char* name)
	{
		for(auto& m : meshes)
		{
			if(m.second.lock()->CompareName(name) && !m.second.expired())
			{
				return m.second.lock();
			}
		}
		return nullptr;
	}

	std::shared_ptr<Mesh::SkinnedMesh> ResourceManager::GetSkinnedMesh(const char* name)
	{
		for(auto& m : skinnedMeshes)
		{
			if(m.second.lock()->CompareName(name) && !m.second.expired())
			{
			//	return m.second.lock();
			}
		}
		return nullptr;
	}

	void ResourceManager::DrawDebug()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2( 0, 50), ImGuiCond_::ImGuiCond_Once);
		ImGui::Begin("ResourceManager", nullptr, ImGuiWindowFlags_MenuBar);
		for(size_t i = 0; i < fbxResources.size(); ++i)
		{
			fbxResources.at(i).DrawDebug();
		}

		for(auto& m : materials)
		{
			if(ImGui::TreeNode(m.second.lock()->GetName()))
			{
			ImGui::Text(m.second.lock()->textureNames[0].c_str());
			ImGui::Text(m.second.lock()->textureNames[1].c_str());
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	std::shared_ptr<Texture> ResourceManager::FindResourceFromFilePath(const char* filePath) const
	{
		for (auto& res : textures)
		{
			if (res.second.lock()->CompareName(filePath) && !res.second.expired())
			{
				return res.second.lock();
			}
		}
		return nullptr;
	}

	
}
