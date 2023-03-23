#include "ArStaticMesh.h"
#include "../Graphic/ArGraphics.h"



namespace Argent::Resource::Mesh
{
	ArStaticMesh::ArStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		this->vertices = vertices;
		this->indices = indices;
		ID3D12Device* device = Graphics::ArGraphics::Instance()->GetDevice();
		vertexBuffer = std::make_unique<Argent::Dx12::ArVertexBuffer<Vertex>>(device, vertices);
		indexBuffer = std::make_unique<Argent::Dx12::ArIndexBuffer>(device, indices);
	}

	ArStaticMesh::ArStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
		const std::vector<Subset>& subsets)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->subsets = subsets;
		ID3D12Device* device = Graphics::ArGraphics::Instance()->GetDevice();
		vertexBuffer = std::make_unique<Argent::Dx12::ArVertexBuffer<Vertex>>(device, vertices);
		indexBuffer = std::make_unique<Argent::Dx12::ArIndexBuffer>(device, indices);
	}

	void ArStaticMesh::Render(ID3D12GraphicsCommandList* cmdList)
	{
//<<<<<<< HEAD
	Mesh::ArMesh<Vertex>::Render(cmdList);

	}

	void ArStaticMesh::Render(ID3D12GraphicsCommandList* cmdList, const DirectX::XMFLOAT4X4& world,
	                          const DirectX::XMFLOAT4& color, UINT instanceCount, UINT indexOffset, INT vertexOffset,
	                          UINT instanceOffset) const
	{
//=======
	//	constantBuffer->SetOnCommandList(cmdList, 1);
//>>>>>>> GinNote
		//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ArMesh<Vertex>::Render(cmdList);
	}

	//void ArStaticMesh::Render(ID3D12GraphicsCommandList* cmdList, const DirectX::XMFLOAT4X4& world,
	//                          const DirectX::XMFLOAT4& color, UINT instanceCount, UINT indexOffset, INT vertexOffset,
	//                          UINT instanceOffset) const
	//{
	//	UpdateConstant(world, color);
	//	constantBuffer->SetOnCommandList(cmdList, 1);
	//	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	ArMesh::SetOnCommandList(cmdList);
	//}
}
