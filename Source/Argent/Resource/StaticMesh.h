#pragma once
#include "ArMesh.h"
#include <vector>
#include <DirectXMath.h>

#include "../Other/CerealHelper.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>


namespace Argent::Resource::Mesh
{
	class ArStaticMesh:
		public ArMesh
	{
	public:
		struct Subset
		{
			uint64_t materialUniqueId{};
			uint32_t startIndexLocation{};
			uint32_t indexCount{};

			template<class T>
			void serialize(T& archive)
			{
				archive(materialUniqueId, startIndexLocation, indexCount);
			}
		};


		DirectX::XMFLOAT4X4 defaultGlobalTransform
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
public:
		ArStaticMesh(const char* name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<Subset>& subsets, const DirectX::XMFLOAT4X4 globalTransform);


		ArStaticMesh(const char* name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<Subset>& subsets); 


		virtual ~ArStaticMesh() override = default;
		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) override;

	public:
		std::vector<Subset> subsets;
	};

}
