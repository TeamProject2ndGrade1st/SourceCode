#pragma once
#include "ArResource.h"
#include "SkinnedMesh.h"
#include "Animation.h"

namespace Argent::Resource::FBX
{
	class FbxResource:
		public Resource::ArImportedResource
	{
		FbxResource(const char* filepath):
			ArImportedResource(filepath, ResourceType::Fbx)
		{}


		std::vector<std::shared_ptr<Mesh::ArSkinnedMesh>> meshes;
		std::vector<Animation::AnimationClip> animationClips;
	};
}
