#pragma once
#include "ArMesh.h"
#include <vector>
#include <DirectXMath.h>
#include "../Graphic/Dx12/ArConstantBuffer.h"

namespace Argent::Mesh::StaticMesh
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	class ArStaticMesh:
		public ArMesh<Vertex>
	{
	public:
		struct Subset
		{
			uint64_t materialUniqueId{};
			uint32_t startIndexLocation{};
			uint32_t indexCount{};
		};
public:
		
		ArStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		ArStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<Subset>& subsets);

		void Render(ID3D12GraphicsCommandList* cmdList) override;

		void Render(ID3D12GraphicsCommandList* cmdList, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& color,
			UINT instanceCount = 1, UINT indexOffset = 0, INT vertexOffset = 0, UINT instanceOffset = 0) const;

	public:
		std::vector<Subset> subsets;
	};

}
