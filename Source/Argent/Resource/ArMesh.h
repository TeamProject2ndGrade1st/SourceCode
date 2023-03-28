#pragma once
#include <d3d12.h>
#include "../Graphic/Dx12/ArVertexBuffer.h"
#include "../Graphic/Dx12/ArIndexBuffer.h"
#include "ArMaterial.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

namespace Argent::Resource::Mesh
{
	/**
	 * \brief ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@������
	 *
	 * \tparam T Vertex�\���̂��w��
	 */
	template<class T>
	class ArMesh
	{
	public:
		ArMesh() = default;
		virtual ~ArMesh() = default;

		virtual void SetOnCommandList(ID3D12GraphicsCommandList* cmdList)
		{
			SetOnCommandList(cmdList, 0, 1);
		}

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT vertexStartSlot, UINT numVertexViews) const
		{
			vertexBuffer->SetOnCommandList(cmdList, vertexStartSlot);
			indexBuffer->SetOnCommandList(cmdList);
		}

		static void DrawCall(ID3D12GraphicsCommandList* cmdList,UINT indexCountPerInstance, UINT instanceCount = 1,
		                     UINT startIndexLocation = 0, UINT baseVertexLocation = 0, UINT startInstanceLocation = 0)
		{
			cmdList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, 
				startIndexLocation, baseVertexLocation, startInstanceLocation);
		}
	protected:
	public:
		//hack �}���`�X���b�h�̎���unique_ptr�ł����v���H
		std::unique_ptr<Argent::Dx12::ArVertexBuffer<T>> vertexBuffer;
		std::unique_ptr<Argent::Dx12::ArIndexBuffer> indexBuffer;
		std::vector<T> vertices;
		std::vector<uint32_t> indices;
	};
}

