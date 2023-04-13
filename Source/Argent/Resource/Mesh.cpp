#include "Mesh.h"
#include "../Graphic/Graphics.h"

namespace Argent::Resource::Mesh
{
	ArMesh::ArMesh(const char* name, const MeshResource& mResource,
	               const std::vector<Subset>& subsets, const DirectX::XMFLOAT4X4 globalTransform) :
		ArResource(name, ResourceType::Mesh)
	,	meshResource(mResource)
	,	subsets(subsets)
	,	localTransform(globalTransform)
	{
		ID3D12Device* device = Graphics::ArGraphics::Instance()->GetDevice();
		vertexBuffer = std::make_unique<Dx12::ArVertexBuffer<Vertex>>(device, mResource.vertices);
		indexBuffer = std::make_unique<Dx12::ArIndexBuffer>(device, mResource.indices);
	}
}
