#include "ArStaticMeshRenderer.h"
#include "../Graphic/ArGraphics.h"

void Argent::Component::Renderer::ArStaticMeshRenderer::Render() const
{
	ID3D12GraphicsCommandList* cmdList = Argent::Graphics::ArGraphics::Instance()->GetCommandList();
	ArRenderer::Render();
	meshData->Render(cmdList);
}
