#include "ResourceManager.h"
#include "ArTexture.h"

namespace Argent::Resource
{
	void ArResourceManager::Initialize()
	{
		Clear();
	
	//	meshData["Cube"] = Primitive::CreateCube();
	//	meshData["Sphere"] = Primitive::CreateSphere();
	//	meshData["Capsule"] = Primitive::CreateCapsule();
	}

	void ArResourceManager::Clear()
	{
		//meshData.clear();
	}

	void ArResourceManager::Begin()
	{
	}

	uint64_t ArResourceManager::LoadTexture(const char* filePath)
	{
		std::shared_ptr<Texture::ArTexture> ret = FindResourceFromFilePath(filePath);
		if(ret) 
			return ret->GetUniqueId();

		ret = std::make_shared<Argent::Texture::ArTexture>(filePath);

		resources[ret->GetUniqueId()] = ret;

		rowTextureData.emplace_back(ret);

		return ret->GetUniqueId();
	}

	//bool ArResourceManager::FindTexture(const wchar_t* filepath, ID3D12Resource** ppResource)
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
