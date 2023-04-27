#include "ResourceManager.h"

namespace Argent::Resource
{
	void ResourceManager::Initialize()
	{
		Clear();
	
	//	meshData["Cube"] = Primitive::CreateCube();
	//	meshData["Sphere"] = Primitive::CreateSphere();
	//	meshData["Capsule"] = Primitive::CreateCapsule();
	}

	void ResourceManager::Clear()
	{
		//meshData.clear();
	}

	void ResourceManager::Begin()
	{
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
