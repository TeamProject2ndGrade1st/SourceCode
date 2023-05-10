#pragma once
#include "RenderingPipeline.h"
#include "Dx12/Descriptor.h"

class LuminanceExtraction
{
	LuminanceExtraction();
private:
	std::unique_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;	//高輝度成分抽出の結果を格納するバッファ
	Argent::Dx12::Descriptor* descriptor;
};

