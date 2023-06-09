#include "SkinnedMesh.h"
#include "../Graphic/Graphics.h"

namespace
	Argent::Resource::Mesh
{
	SkinnedMesh::SkinnedMesh(const char* name, const MeshResource& mResource, std::vector<BoneVertex> bones,
		const std::vector<Subset>& subsets, const Skeleton& bindPose):
		Mesh(name	, mResource, subsets, DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1))
	,	bindPose(bindPose)
	{
		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();
		boneVertexBuffer = std::make_unique<Dx12::ArVertexBuffer<BoneVertex>>(
			device, bones);
		constantBuffer = std::make_unique<Dx12::ConstantBuffer<Constant>>(
			device,
			Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor());
	}

	void SkinnedMesh::SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT vertexStartSlot)
	{
		boneVertexBuffer->SetOnCommandList(cmdList, 1);
		Mesh::SetOnCommandList(cmdList, vertexStartSlot);
	}
}
