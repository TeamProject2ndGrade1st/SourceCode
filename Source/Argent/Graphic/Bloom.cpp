#include "Bloom.h"

void Bloom::Init(ID3D12Device* device, D3D12_RESOURCE_DESC resDesc, float clearColor[4])
{
	renderingPipeline = Argent::Graphics::RenderingPipeline::CreateBloomPipeline();

	luminanceExtraction = std::make_unique<LuminanceExtraction>(device, resDesc, clearColor);
	gaussianBlur = std::make_unique<Argent::Graphics::GaussianBlur>(device, resDesc, clearColor);
}

void Bloom::Execute(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, 
	D3D12_VIEWPORT viewport, D3D12_RECT scissorRect)
{
	//‚‹P“x¬•ª‚ð’Šo‚·‚é

	luminanceExtraction->Draw(cmdList,
	scissorRect, gpuHandle, viewport);

	gaussianBlur->Execute(cmdList, scissorRect, luminanceExtraction->GetSrvGPUHandle());
}

void Bloom::Draw(ID3D12GraphicsCommandList* cmdList)
{
	renderingPipeline->SetOnCommandList(cmdList);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	gaussianBlur->SetOnCommandList(cmdList, 0);
	cmdList->DrawInstanced(4, 1, 0, 0);

}
