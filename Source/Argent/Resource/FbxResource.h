#pragma once
#include "ArResource.h"
#include "SkinnedMesh.h"
#include "Animation.h"

namespace Argent::Resource::Fbx
{
	struct ArBoneInfluence
	{
		uint32_t boneIndex;
		float boneWeight;
	};

	struct ArFbxScene
	{
		struct Node
		{
			uint64_t id{};
			std::string name;
			FbxNodeAttribute::EType attribute{};
			int64_t parentIndex{ -1 };

			template<class T>
			void serialize(T& archive)
			{
				archive(id, name, attribute, parentIndex);
			}
		};
		std::vector<Node> nodes{};

		//todo 何のやつでしょう？
		int64_t IndexOf(uint64_t id) const  // NOLINT(modernize-use-nodiscard)
		{
			int64_t index{};
			for(const Node& node : nodes)
			{
				if(node.id == id) return index;
				++index;
			}
			return -1;
		}

		template<class T>
		void serialize(T& archive)
		{
			archive(nodes);
		}
	};

	struct TmpFbxMesh
	{
		int64_t nodeIndex;
		std::string name;
		Argent::Resource::Mesh::MeshResource meshResource;
		std::vector<Resource::Mesh::BoneVertex> vertexBones;
		std::vector<Resource::Mesh::Subset> subsets;
		Argent::Resource::Mesh::Skeleton bindPose;
		DirectX::XMFLOAT4X4 defaultGlobalTransform
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};

		template<class T>
		void serialize(T& archive)
		{
			archive(nodeIndex, name, meshResource, vertexBones, subsets, bindPose, defaultGlobalTransform);
		}
	};

	struct ReplaceFileName
	{
		std::string replace[2];
	};
	struct FbxResource
	{
		std::string filePath;
		std::vector<TmpFbxMesh> tmpMeshes;
		std::unordered_map<std::string, std::shared_ptr<Material::MeshMaterial>> materials;
		std::vector<Resource::Animation::AnimationClip> animationClips;

		std::vector<ReplaceFileName> replaceTextureFileName;
		bool isResize = false; //上のストリングリサイズ用
		void DrawDebug();
		template<class T>
		void serialize(T& archive)
		{
			archive(filePath, tmpMeshes, materials, animationClips);
		}
	};
}
