#pragma once

#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>

namespace Argent::Loader
{
	namespace Fbx
	{
		struct ArFbxScene;

		struct BoneInfluence
		{
			uint32_t boneIndex;
			float boneWeight;
		};

		struct ArSkeleton
		{
			struct Bone
			{
				uint64_t uniqueId{};
				std::string name;
				int64_t parentIndex{ -1 };
				int64_t nodeIndex{};

				DirectX::XMFLOAT4X4 offsetTransform
				{
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
				};
				bool isOrphan() const { return parentIndex < 0; }
			};
			std::vector<Bone> bones{};
			int64_t indexOf(uint64_t uniqueId) const
			{
				int64_t index{};
				for(const Bone& bone : bones)
				{
					if(bone.uniqueId == uniqueId) return index;
					++index;
				}
				return -1;
			}
		};

		struct ArAnimation
		{
			std::string name;
			float samplingRate{};

			struct ArKeyframe
			{
				struct Node
				{
					DirectX::XMFLOAT4X4 globalTransform
					{
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
					};

					DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
					DirectX::XMFLOAT4 rotation{ 0, 0, 0, 1 };
					DirectX::XMFLOAT3 translation{ 0, 0, 0 };
				};
				std::vector<Node> nodes;
			};
			std::vector<ArKeyframe> sequence;
		};

		void LoadFbx(const char* fileName, bool triangulate = false);

	}
}
