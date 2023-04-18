#include "DebugRenderer.h"
#include "../GameObject/GameObject.h"

namespace Argent::Debug
{
	void DebugRenderer::Render(const DirectX::XMFLOAT4X4& world)
	{
		ID3D12GraphicsCommandList* cmdList = Graphics::Graphics::Instance()->GetCommandList(Graphics::RenderType::Mesh);
		renderingPipeline->SetOnCommandList(cmdList);
		Argent::Graphics::Graphics::Instance()->SetSceneConstant(0);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Constants constant{};
		constant.world = world;
		constantBuffer->UpdateConstantBuffer(constant);
		constantBuffer->SetOnCommandList(cmdList, 1);

		vertexBuffer->SetOnCommandList(cmdList, 0);
		indexBuffer->SetOnCommandList(cmdList);
		cmdList->DrawIndexedInstanced(static_cast<UINT>(meshResource.indices.size()), 1, 0, 0, 0);
		
	}

	void Debug::Render() const
	{
		renderer->Render(GetOwner()->GetTransform()->GetWorld());
	}
}
