#pragma once
#include <DirectXMath.h>
#include "../Graphic/Dx12/VertexBuffer.h"
#include "../Graphic/Dx12/IndexBuffer.h"
#include "ArResource.h"
#include "Material.h"

namespace Argent::Resource
{
	namespace Sprite
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texcoord;
			DirectX::XMFLOAT4 color;
		};

		//スプライト描画に使う頂点バッファを頂点情報を持ってるクラス
		//hack　こいつにテクスチャのcenter とかtexpos持たせてもいいかも…

		class Sprite:
			public ArResource
		{
		public:
			Sprite();

			/*void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) override
			{
				Mesh::SetOnCommandList(cmdList, 0, 1);
				DrawCall(cmdList, static_cast<UINT>(indices.size()));
			}*/
			/**
				 * \brief 頂点情報の更新用
				 * \param position 座標(Z値は無視される)
				 * \param scale スケール(Z値は無視される)
				 * \param center 回転の中心地
				 * \param angle 回転量
				 * \param textureWidth テクスチャの幅
				 * \param textureHeight テクスチャの高さ
				 * \param color 色
				 */
			void UpdateVertexMap(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT2& center, float angle, float textureWidth, float textureHeight, const DirectX::XMFLOAT4& color) const;

			void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT vertexStartSlot = 0) const
			{
				vertexBuffer->SetOnCommandList(cmdList, vertexStartSlot);
			}


		private:
			std::unique_ptr<Argent::Dx12::ArVertexBuffer<Vertex>> vertexBuffer{};
			std::vector<Vertex> vertices;
			Vertex* vertexMap{};
		};
	}

}

