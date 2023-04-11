#pragma once
#include "ArResource.h"
#include "SkinnedMesh.h"
#include "ArAnimation.h"

namespace Argent::Resource::FBX
{
	class FbxResource:
		public Resource::ArImportedResource
	{
		FbxResource(const char* filepath):
			ArImportedResource(filepath, ResourceType::Fbx)
		{}


		std::vector<std::shared_ptr<Mesh::ArSkinnedMesh>> meshes;
		std::vector<Animation::ArAnimation> animationClips;
	};
}
