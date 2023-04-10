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
		DirectX::XMFLOAT2 texcoord;

		template<class T>
		void serialize(T& archive)
		{
			archive(position, normal, texcoord);
		}
	};

	struct MeshResource
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
	};

	/**
	 * \brief 頂点バッファとインデックスバッファを持つ
	 *
	 */
	class ArMesh:
		public ArResource
	{
	public:
		ArMesh(const char* name, ResourceType type):
			ArResource(name, type)
		{}
		virtual ~ArMesh() override = default;

		virtual void SetOnCommandList(ID3D12GraphicsCommandList* cmdList)
		{
			SetOnCommandList(cmdList, 0, 1);
		}

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT vertexStartSlot, UINT numVertexViews) const
		{
			vertexBuffer->SetOnCommandList(cmdList, vertexStartSlot);
			indexBuffer->SetOnCommandList(cmdList);
		}
	public:
		//hack マルチスレッドの時にunique_ptrでも大丈夫か？
		std::unique_ptr<Argent::Dx12::ArVertexBuffer<Vertex>> vertexBuffer{};
		std::unique_ptr<Argent::Dx12::ArIndexBuffer> indexBuffer{};
		MeshResource meshResource;
	};

}

