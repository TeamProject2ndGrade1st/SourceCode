#pragma once

#include <DirectXMath.h>
#include <vector>

#include "../Graphic/Graphics.h"
#include "../Graphic/Dx12/VertexBuffer.h"
#include "../Graphic/Dx12/IndexBuffer.h"
#include "../Graphic/RenderingPipeline.h"
#include "../Resource/Mesh.h"
#include "../Component/MeshRenderer.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

#include <fstream>

#include "../GameObject/GameObject.h"

//判定用のメッシュ表示用クラス
namespace Argent::Debug
{
	class DebugRenderer
	{
	public:
		using Vertex = Argent::Resource::Mesh::Vertex;
		using MeshResource = Argent::Resource::Mesh::MeshResource;
		using Constants = Argent::Component::Renderer::MeshRenderer::Constants;

		DebugRenderer(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices)
		{
			meshResource.vertices = vertices;
			meshResource.indices = indices;
			auto device = Graphics::ArGraphics::Instance()->GetDevice();
			vertexBuffer = std::make_unique<Dx12::ArVertexBuffer<Vertex>>(device, vertices);
			indexBuffer = std::make_unique<Dx12::ArIndexBuffer>(device, indices);
			renderingPipeline = Graphics::RenderingPipeline::CreateDebugRendererPipeLine();
			constantBuffer = std::make_unique<Argent::Dx12::ArConstantBuffer<Constants>>(device, Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor());

		}
		DebugRenderer(const MeshResource& mResource) :
			meshResource(mResource)
		{
			auto device = Graphics::ArGraphics::Instance()->GetDevice();
			vertexBuffer = std::make_unique<Dx12::ArVertexBuffer<Vertex>>(device, meshResource.vertices);
			indexBuffer = std::make_unique<Dx12::ArIndexBuffer>(device, meshResource.indices);
			renderingPipeline = Graphics::RenderingPipeline::CreateDebugRendererPipeLine();
			constantBuffer = std::make_unique<Argent::Dx12::ArConstantBuffer<Constants>>(device, Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor());
		}

		void Render(const DirectX::XMFLOAT4X4& world);

		MeshResource meshResource;
		//std::vector<Vertex> vertices;
		//std::vector<uint32_t> indices;

		std::unique_ptr<Argent::Dx12::ArVertexBuffer<Vertex>> vertexBuffer;
		std::unique_ptr<Argent::Dx12::ArIndexBuffer> indexBuffer;

		std::shared_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline;
		std::unique_ptr<Argent::Dx12::ArConstantBuffer<Constants>> constantBuffer{};

	};

	class Debug : public Argent::Component::BaseComponent
	{
	public:
		Debug() :
			BaseComponent("Debug")
		{
			std::ifstream ifs("./Resources/Model/Cylinder.cereal", std::ios::binary);
			cereal::BinaryInputArchive deserialization(ifs);
			Argent::Resource::Mesh::MeshResource mResource;
			deserialization(mResource);
			renderer = std::make_unique<DebugRenderer>(mResource);
		};

		void Render() const override
		{
			renderer->Render(GetOwner()->GetTransform()->GetWorld());
		}

		std::unique_ptr<DebugRenderer> renderer;
	};
}
