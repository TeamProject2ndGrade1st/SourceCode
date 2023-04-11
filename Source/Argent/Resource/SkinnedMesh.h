#pragma once
#include "Mesh.h"
#include "../Other/CerealHelper.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>


namespace Argent::Resource::Mesh
{
	struct Skeleton
	{
		struct Bone
		{
			template<class T>
			void serialize(T& archive)
			{
				archive(uniqueId, name, parentIndex, nodeIndex, offsetTransform);
			}



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
			for (const Bone& bone : bones)
			{
				if (bone.uniqueId == uniqueId) return index;
				++index;
			}
			return -1;
		}

		template<class T>
		void serialize(T& archive)
		{
			archive(bones);
		}
	};
	static constexpr int MaxBoneInfluences{ 4 };
	struct VertexBone
	{
		float boneWeights[MaxBoneInfluences]{ 1, 0, 0, 0 };
		uint32_t boneIndices[MaxBoneInfluences];

		template<class T>
		void serialize(T& archive)
		{
			archive(boneWeights, boneIndices);
		}
	};
	class ArSkinnedMesh final :
		public ArMesh
	{
	public:
		static const int MaxBones{ 256 };
		ArSkinnedMesh(const char* name, const std::vector<Vertex>& vertices, std::vector<VertexBone> bones,  const std::vector<uint32_t>& indices,
			const std::vector<Subset>& subsets, const Skeleton& bindPose);
		~ArSkinnedMesh() override = default;

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) override;

		std::unique_ptr<Argent::Dx12::ArVertexBuffer<VertexBone>> boneVertexBuffer;
		Skeleton bindPose;

		struct Constant
		{
			DirectX::XMFLOAT4X4 globalTransform;
			DirectX::XMFLOAT4X4 boneTransforms[MaxBones]
			{
				{1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
				}
			};
		};

		int64_t nodeIndex{};

		std::unique_ptr<Argent::Dx12::ArConstantBuffer<Constant>> constantBuffer;
	};
}