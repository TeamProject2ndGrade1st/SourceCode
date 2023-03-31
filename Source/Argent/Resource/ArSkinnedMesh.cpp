#include "ArSkinnedMesh.h"
#include "../Graphic/ArGraphics.h"

namespace
	Argent::Resource::Mesh
{
	ArSkinnedMesh::ArSkinnedMesh(const char* name, const std::vector<Vertex>& vertices,
		std::vector<VertexBone> bones,
		const std::vector<uint32_t>& indices, const std::vector<Subset>& subsets,
		const Skeleton& bindPose) :
		ArStaticMesh(name, vertices, indices, subsets)
	,	bindPose(bindPose)
	{
		ID3D12Device* device = Argent::Graphics::ArGraphics::Instance()->GetDevice();
		boneVertexBuffer = std::make_unique<Argent::Dx12::ArVertexBuffer<VertexBone>>(
			device, bones);
		constantBuffer = std::make_unique<Argent::Dx12::ArConstantBuffer<Constant>>(
			device,
			Argent::Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor()
			);
	}

	void ArSkinnedMesh::SetOnCommandList(ID3D12GraphicsCommandList* cmdList)
	{
		boneVertexBuffer->SetOnCommandList(cmdList, 1);
		//constantBuffer->SetOnCommandList(cmdList, 2);
		ArStaticMesh::SetOnCommandList(cmdList);
	}
}
