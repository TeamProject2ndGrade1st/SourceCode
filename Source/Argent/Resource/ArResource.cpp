#include "ArResource.h"
#include "ResourceManager.h"

namespace Argent::Resource
{
	ArResource::ArResource(const char* name, ResourceType type):
		uniqueId(ResourceManager::GenerateResourceUniqueId())
	,	name(name)
	,	type(type)
	{}

	ArResource::ArResource(ResourceType type):
		uniqueId(ResourceManager::GenerateResourceUniqueId())
	,	name("Not Setting")
	,	type(type)
	{}
}
