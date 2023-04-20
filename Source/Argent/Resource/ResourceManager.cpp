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

	//bool ResourceManager::FindTexture(const wchar_t* filepath, ID3D12Resource** ppResource)
	//{
	//	const std::wstring tmp = filepath;
	//	const auto it = textures.find(tmp);

	//	if (it != textures.end())
	//	{
	//		*ppResource = (*it).second.Get();
	//		(*it).second->AddRef();
	//		return true;
	//	}

	//	*ppResource = nullptr;
	//	return false;
	//}
	
}
