#include "ArResource.h"
#include "ArResourceManager.h"

namespace Argent::Resource
{
	ArResource::ArResource(const char* name, ResourceType type):
		uniqueId(ArResourceManager::GenerateResourceUniqueId())
	,	name(name)
	,	type(type)
	{}

	ArResource::ArResource(ResourceType type):
		uniqueId(ArResourceManager::GenerateResourceUniqueId())
	,	name("Not Setting")
	,	type(type)
	{}
}
