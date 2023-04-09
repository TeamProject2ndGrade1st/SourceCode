#pragma once
#include <filesystem>
#include <string>
#include "../Other/Helper.h"

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
			Animation,
			Fbx
		};
		ArResource(const char* name, ResourceType type);
		ArResource(ResourceType type);

	protected:
		ArResource(uint64_t uniqueId, const char* name, ResourceType type):
			uniqueId(uniqueId),
			name(name),
			type(type)
		{}
	public:
		virtual ~ArResource() = default;

		virtual bool CompareName(const char* n)
		{
			return name == n;
		}

		uint64_t GetUniqueId() const { return uniqueId; }  // NOLINT(modernize-use-nodiscard)
		const char* GetName() const { return name.c_str(); }// NOLINT(modernize-use-nodiscard)
		void SetName(const char* n) { name = n; }
	private:
		uint64_t uniqueId;
		std::string name;
		const ResourceType type;
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

	protected:
		std::string filePath;
	};
}