#include "ArSkinnedMesh.h"
#include "../Graphic/ArGraphics.h"

namespace
	Argent::Resource::Mesh
{
	ArSkinnedMesh::ArSkinnedMesh(const std::vector<Vertex>& vertices,
		std::vector<VertexBone> bones,
		const std::vector<uint32_t>& indices, const std::vector<Subset>& subsets,
		const Skeleton& bindPose) :
		ArStaticMesh(vertices, indices, subsets)
	,	bindPose(bindPose)
	{
		ID3D12Device* device = Argent::Graphics::ArGraphics::Instance()->GetDevice();
		boneVertexBuffer = std::make_unique<Argent::Dx12::ArVertexBuffer<VertexBone>>(
			device, bones);
	}

	void ArSkinnedMesh::SetOnCommandList(ID3D12GraphicsCommandList* cmdList)
	{
		ArStaticMesh::SetOnCommandList(cmdList);
		boneVertexBuffer->SetOnCommandList(cmdList, 1);
	}
}
