#include "DebugRenderer.h"

namespace Argent::Debug
{
	void DebugRenderer::Render(const DirectX::XMFLOAT4X4& world)
	{
		ID3D12GraphicsCommandList* cmdList = Graphics::ArGraphics::Instance()->GetCommandList();
		renderingPipeline->SetOnCommandList(cmdList);
		Argent::Graphics::ArGraphics::Instance()->SetSceneConstant(0);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Constants constant{};
		constant.world = world;
		constantBuffer->UpdateConstantBuffer(constant);
		constantBuffer->SetOnCommandList(cmdList, 1);

		vertexBuffer->SetOnCommandList(cmdList, 0);
		indexBuffer->SetOnCommandList(cmdList);
		cmdList->DrawIndexedInstanced(meshResource.indices.size(), 1, 0, 0, 0);
		
	}
}