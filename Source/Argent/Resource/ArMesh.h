#pragma once
#include <d3d12.h>
#include "../Graphic/Dx12/ArVertexBuffer.h"
#include "../Graphic/Dx12/ArIndexBuffer.h"
#include "ArMaterial.h"


namespace Argent::Resource::Mesh
{
	/**
	 * \brief 頂点バッファとインデックスバッファを持つ
	 *
	 * \tparam T Vertex構造体を指定
	 */
	template<class T>
	class ArMesh
	{
	public:
		ArMesh() = default;
		virtual ~ArMesh() = default;

		virtual void Render(ID3D12GraphicsCommandList* cmdList)
		{
			SetOnCommandList(cmdList);
			DrawCall(cmdList, indices.size());
		}

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT vertexStartSlot = 0, UINT numVertexViews = 1) const
		{
			vertexBuffer->SetOnCommandList(cmdList, vertexStartSlot, numVertexViews);
			indexBuffer->SetOnCommandList(cmdList);
		}

		static void DrawCall(ID3D12GraphicsCommandList* cmdList,UINT indexCountPerInstance, UINT instanceCount = 1,
		                     UINT startIndexLocation = 0, UINT baseVertexLocation = 0, UINT startInstanceLocation = 0)
		{
			cmdList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, 
				startIndexLocation, baseVertexLocation, startInstanceLocation);
		}
	protected:

		//hack マルチスレッドの時にunique_ptrでも大丈夫か？
		std::unique_ptr<Argent::Dx12::ArVertexBuffer<T>> vertexBuffer;
		std::unique_ptr<Argent::Dx12::ArIndexBuffer> indexBuffer;
		std::vector<T> vertices;
		std::vector<uint32_t> indices;
	};
}

