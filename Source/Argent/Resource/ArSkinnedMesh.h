#pragma once
#include "ArStaticMesh.h"

namespace Argent::Resource::Mesh
{
	struct Skeleton
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

		std::vector<Bone> bones;
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
	};

	struct VertexBone
	{
		
	};
	class ArSkinnedMesh final :
		public ArStaticMesh
	{
	public:
		ArSkinnedMesh(const std::vector<Vertex>& vertices, std::vector<VertexBone> bones,  const std::vector<uint32_t>& indices,
			const std::vector<Subset>& subsets, const Skeleton& bindPose);
		~ArSkinnedMesh() override = default;

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) override;

		std::unique_ptr<Argent::Dx12::ArVertexBuffer<VertexBone>> boneVertexBuffer;
		Skeleton bindPose;
	};
}
