#pragma once

#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>

#include "../Resource/Mesh.h"
#include "../Resource/SkinnedMesh.h"
#include "../Resource/ArAnimation.h"

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

			//todo âΩÇÃÇ‚Ç¬Ç≈ÇµÇÂÇ§ÅH
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
			std::vector<Argent::Resource::Mesh::Vertex> vertices;
			std::vector<Resource::Mesh::VertexBone> vertexBones;
			std::vector<uint32_t> indices;
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
				archive(nodeIndex, name, vertices, vertexBones, indices, subsets, bindPose, defaultGlobalTransform);
			}
		};


		struct FbxResource
		{
			std::vector<TmpFbxMesh> tmpMeshes;
			std::unordered_map<uint64_t, Material::ArMeshMaterial> materials;
			std::vector<Resource::Animation::ArAnimation> animationClips;

			template<class T>
			void serialize(T& archive)
			{
				archive(tmpMeshes, materials, animationClips);
			}
		};
		std::vector<Component::BaseComponent*> LoadFbx(const char* filePath, bool triangulate = false);
		

	}
}
