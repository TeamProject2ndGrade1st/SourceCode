#include "ArStaticMesh.h"
#include "../Graphic/ArGraphics.h"


namespace Argent::Resource::Mesh
{
	ArStaticMesh::ArStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
		const std::vector<Subset>& subsets, const DirectX::XMFLOAT4X4 globalTransform):
		defaultGlobalTransform(globalTransform)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->subsets = subsets;
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

	void ArStaticMesh::SetOnCommandList(ID3D12GraphicsCommandList* cmdList)
	{
		Mesh::ArMesh<Vertex>::SetOnCommandList(cmdList);
	}
}
