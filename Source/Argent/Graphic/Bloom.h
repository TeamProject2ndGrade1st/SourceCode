#pragma once
#include "RenderingPipeline.h"
//todo そのうちやる
class Bloom
{
public:
	Bloom();

	void Begin(ID3D12GraphicsCommandList* cmdList);

private:

	std::unique_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline;
};

