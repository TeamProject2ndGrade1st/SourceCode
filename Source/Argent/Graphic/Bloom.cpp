#include "Bloom.h"

Bloom::Bloom()
{
	renderingPipeline = Argent::Graphics::RenderingPipeline::CreateBloomPipeline();
}

void Bloom::Begin(ID3D12GraphicsCommandList* cmdList)
{
	renderingPipeline->SetOnCommandList(cmdList);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
