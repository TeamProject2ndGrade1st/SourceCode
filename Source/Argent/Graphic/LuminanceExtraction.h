#pragma once
#include "RenderingPipeline.h"
#include "Dx12/Descriptor.h"

class LuminanceExtraction
{
	LuminanceExtraction();
private:
	std::unique_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;	//���P�x�������o�̌��ʂ��i�[����o�b�t�@
	Argent::Dx12::Descriptor* descriptor;
};

