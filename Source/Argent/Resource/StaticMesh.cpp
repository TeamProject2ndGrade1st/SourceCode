#include "StaticMesh.h"
#include "../Graphic/Graphics.h"


namespace Argent::Resource::Mesh
{
	ArStaticMesh::ArStaticMesh(const char* name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
		const std::vector<Subset>& subsets, const DirectX::XMFLOAT4X4 globalTransform) :
		ArMesh(name, ResourceType::Mesh)
	,	defaultGlobalTransform(globalTransform)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->subsets = subsets;
		ID3D12Device* device = Graphics::ArGraphics::Instance()->GetDevice();
		vertexBuffer = std::make_unique<Argent::Dx12::ArVertexBuffer<Vertex>>(device, vertices);
		indexBuffer = std::make_unique<Argent::Dx12::ArIndexBuffer>(device, indices);
	}

	ArStaticMesh::ArStaticMesh(const char* name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
		const std::vector<Subset>& subsets):
		ArMesh(name, ResourceType::Mesh)
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
