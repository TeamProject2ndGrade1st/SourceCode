#pragma once
#include "RenderingPipeline.h"
#include "GaussianBlur.h"
#include "LuminanceExtraction.h"
//todo ‚»‚Ì‚¤‚¿‚â‚é
class Bloom
{
public:
	Bloom() = default;

	void Init(ID3D12Device* device, D3D12_RESOURCE_DESC resDesc, float clearColor[4]);
	void Execute(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, 
	D3D12_VIEWPORT viewport, D3D12_RECT scissorRect);

	void Draw(ID3D12GraphicsCommandList* cmdList);

private:

	std::unique_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline;
	std::unique_ptr<Argent::Graphics::GaussianBlur> gaussianBlur;
	std::unique_ptr<LuminanceExtraction> luminanceExtraction;
};

