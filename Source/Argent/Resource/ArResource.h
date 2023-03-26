#pragma once
#include <filesystem>
#include <string>
#include "../Other/ArHelper.h"

namespace Argent::Resource
{
	class ArResource
	{
	public:

		enum class ResourceType
		{
			Shader,
			Texture,
			Mesh,
			SkinnedMesh,
			Material,
			Effect,
			Audio,
		};
		ArResource(const char* name, ResourceType type);

		virtual bool CompareName(const char* n)
		{
			return name == n;
		}

		uint64_t GetUniqueId() const { return uniqueId; }  // NOLINT(modernize-use-nodiscard)
		const char* GetName() const { return name.c_str(); }// NOLINT(modernize-use-nodiscard)

	private:
		uint64_t uniqueId;
		std::string name;
		ResourceType type;
	};

	class ArImportedResource:
		public ArResource
	{
	public:
		ArImportedResource(const char* filePath, ResourceType type):
			ArResource(Argent::Helper::String::ExtractFileName(filePath, false).c_str(), type)
		,	filePath(filePath)
		{}
		virtual ~ArImportedResource() = default;

		const char* GetFilePath() const { return filePath.c_str(); }

		bool CompareName(const char* n) override
		{
			return ArResource::CompareName(n) || filePath == n;
		}

		void AddResource(std::shared_ptr<ArResource> res)
		{
			childResources.emplace_back(res);
		}

	protected:
		std::string filePath;
		std::vector<std::shared_ptr<ArResource>> childResources{};
	};
}