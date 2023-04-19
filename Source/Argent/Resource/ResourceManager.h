#pragma once
#include <unordered_map>
#include <d3d12.h>
#include <memory>

#include "ArResource.h"
#include "Texture.h"
#include "Material.h"



namespace Argent::Resource
{
	class ResourceManager
	{
		static constexpr uint64_t errorUniqueId = 0;
		ResourceManager()
		{
			Clear();
		}
	public:
		~ResourceManager() = default;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(const ResourceManager&&) = delete;
		ResourceManager operator=(const ResourceManager&) = delete;
		ResourceManager operator=(const ResourceManager&&) = delete;

		void Initialize();
		void Clear();
		void Begin();

		/**
		 * \brief リソース管理用ユニークidを生み出す
		 * \return 
		 */
		[[nodiscard]]static UINT64 GenerateResourceUniqueId()
		{
			static UINT64 uniqueId = 1;
			const UINT64 ret = uniqueId;
			++uniqueId;
			return ret;
		}

		std::shared_ptr<ArResource> GetResource(uint64_t uniqueId) const  // NOLINT(modernize-use-nodiscard)
		{
			const auto it = textures.find(uniqueId);
			if(it == textures.end()) _ASSERT_EXPR(FALSE, "missing id");
			if(it->second.expired()) _ASSERT_EXPR(FALSE, "missing resource");

			return it->second.lock();
		}

		std::shared_ptr<Texture> GetTexture(uint64_t uniqueId) const 
		{
			const auto it = textures.find(uniqueId);
			if (it == textures.end()) _ASSERT_EXPR(FALSE, L"missing id");
			if (it->second.expired()) _ASSERT_EXPR(FALSE, L"missing resource");

			return it->second.lock();
		}

		void RegisterMaterial(std::shared_ptr<Material::MeshMaterial> m)
		{
			resources.emplace_back(m);
			materials[m->GetUniqueId()] = m;
		}

		uint64_t LoadTexture(const char* filePath);

		std::shared_ptr<Material::MeshMaterial> GetMaterial(const char* name);

	private:

		std::shared_ptr<Texture> FindResourceFromFilePath(const char* filePath) const;
		

		std::vector<std::shared_ptr<ArResource>> resources;	//データ保持のためっぽいです
		std::unordered_map<uint64_t, std::weak_ptr<Texture>> textures;
		std::unordered_map<uint64_t, std::weak_ptr<Material::MeshMaterial>> materials;


	public:
		static ResourceManager& Instance()
		{
			static ResourceManager instance;
			return instance;
		}
	};
}
