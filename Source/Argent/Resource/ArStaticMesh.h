#pragma once
#include "ArMesh.h"
#include <vector>
#include <DirectXMath.h>
#include "../Graphic/Dx12/ArConstantBuffer.h"

namespace Argent::Resource::Mesh
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
		ArStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<Subset>& subsets);
		virtual ~ArStaticMesh() override = default;
		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) override;

	public:
		std::vector<Subset> subsets;
	};

}
