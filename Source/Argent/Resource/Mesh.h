#pragma once
#include <d3d12.h>
#include "../Graphic/Dx12/VertexBuffer.h"
#include "../Graphic/Dx12/IndexBuffer.h"
#include "ArResource.h"
#include "Material.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

namespace Argent::Resource::Mesh
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 tangent{ 1, 0, 0, 1};
		DirectX::XMFLOAT2 texcoord;

		template<class T>
		void serialize(T& archive)
		{
			archive(position, normal, tangent, texcoord);
		}
	};

	struct Subset
	{
		uint64_t materialUniqueId{};
		uint32_t startIndexLocation{};
		uint32_t indexCount{};

		std::string materialName;
		std::shared_ptr<Material::MeshMaterial> material{};

		template<class T>
		void serialize(T& archive)
		{
			archive(materialUniqueId, startIndexLocation, indexCount, materialName);
		}
	};

	struct MeshResource
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		template<class T>
		void serialize(T& archive)
		{
			archive(vertices, indices);
		}
	};

	/**
	 * \brief 頂点バッファとインデックスバッファを持つ
	 *  
	 */
	class Mesh:
		public ArResource 
	{
	public:
		Mesh(const char* name, const MeshResource& mResource,
		const std::vector<Subset>& subsets, const DirectX::XMFLOAT4X4 globalTransform);
		virtual ~Mesh() override = default;

		virtual void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT vertexStartSlot = 0)
		{
			vertexBuffer->SetOnCommandList(cmdList, vertexStartSlot);
			indexBuffer->SetOnCommandList(cmdList);
		}

		//hack マルチスレッドの時にunique_ptrでも大丈夫か？
		std::unique_ptr<Dx12::ArVertexBuffer<Vertex>> vertexBuffer{};
		std::unique_ptr<Dx12::ArIndexBuffer> indexBuffer{};
		MeshResource meshResource{};
		std::vector<Subset> subsets{};

		DirectX::XMFLOAT4X4 localTransform
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
	};

}

