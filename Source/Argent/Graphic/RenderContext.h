#pragma once
#include <d3d12.h>

namespace Argent::Graphics
{
	class RenderContext final
	{
	public:
		RenderContext() = default;
		~RenderContext() = default;

		void SetCommandList(ID3D12GraphicsCommandList* cmdList) { this->cmdList = cmdList; }
		void Draw();
	private:
		ID3D12GraphicsCommandList* cmdList;
		D3D12_VERTEX_BUFFER_VIEW vView;
		int vertexStartSlot;
		int numView;
		D3D12_INDEX_BUFFER_VIEW iView;
		D3D12_PRIMITIVE_TOPOLOGY primitiveTopology;
	};
}