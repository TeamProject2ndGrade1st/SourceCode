#pragma once
#include "../Resource/Texture.h"
#include "RenderingPipeline.h"
#include "Dx12/ConstantBuffer.h"

class SkyMap
{
public:
	SkyMap() = default;
	void Initialize(const char* filePath, ID3D12Device* device, Argent::Dx12::Descriptor* descriptor);

	void Draw(ID3D12GraphicsCommandList* cmdList, const DirectX::XMFLOAT4X4& constantValue);

private:
	std::shared_ptr<Argent::Resource::Texture> skyMap;
	std::unique_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline;
	std::unique_ptr<Argent::Dx12::ConstantBuffer<DirectX::XMFLOAT4X4>> constant;
};

