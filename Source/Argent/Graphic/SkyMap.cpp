#include "SkyMap.h"
#include "../Resource/ResourceManager.h"
#include "Graphics.h"

using ResourceManager = Argent::Resource::ResourceManager;
void SkyMap::Initialize(const char* filePath, ID3D12Device* device, Argent::Dx12::Descriptor* descriptor)
{
	auto& instance = ResourceManager::Instance();
	skyMap = instance.GetTexture(instance.LoadTexture(filePath));
	renderingPipeline = Argent::Graphics::RenderingPipeline::CreateSkyMapPipeline();
	constant = std::make_unique<Argent::Dx12::ConstantBuffer<DirectX::XMFLOAT4X4>>(device, descriptor);
}

void SkyMap::Draw(ID3D12GraphicsCommandList* cmdList, const DirectX::XMFLOAT4X4& constantValue)
{
	//constant->UpdateConstantBuffer(constantValue);
	renderingPipeline->SetOnCommandList(cmdList);
	//constant->SetOnCommandList(cmdList, 1);
	Argent::Graphics::Graphics::Instance()->SetSceneConstant(1);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	skyMap->SetOnCommandList(cmdList, 0);
	cmdList->DrawInstanced(4, 1, 0, 0);

}

