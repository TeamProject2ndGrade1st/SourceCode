#pragma once

#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>

#include "../Resource/Animation.h"
#include "../Resource/FbxResource.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

namespace Argent::Component
{
	class BaseComponent;
}

namespace Argent::Loader
{
	namespace Fbx
	{
		std::vector<Component::BaseComponent*> LoadFbx(const char* filePath, bool triangulate = false);

		void FetchMesh(FbxScene* fbxScene,const Resource::Fbx::ArFbxScene& sceneView, std::vector<Resource::Fbx::TmpFbxMesh>& meshes);
		void FetchMaterial(FbxScene* fbxScene, const Resource::Fbx::ArFbxScene& sceneView, const char* fbxFilePath, std::unordered_map<std::string, std::shared_ptr<Material::MeshMaterial>>& materials);
		void FetchSkeleton(FbxMesh* fbxMesh, Argent::Resource::Mesh::Skeleton& bindPose, const Resource::Fbx::ArFbxScene& sceneView);
		void FetchAnimation(FbxScene* fbxScene, std::vector<Resource::Animation::AnimationClip>& animationClips, 
			float samplingRate, const Resource::Fbx::ArFbxScene& sceneView);
		void FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<std::vector<Resource::Fbx::ArBoneInfluence>>& boneInfluences);

		void LoadDebug(const char* filePath);

	}
}
