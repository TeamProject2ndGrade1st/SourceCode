#include "Texture.h"

#include "ArResource.h"
#include "../Graphic/Graphics.h"
#include "../Graphic/Dx12/DescriptorHeap.h"
#include "../Other/Helper.h"
#include "../Resource/ResourceManager.h"

namespace Argent::Resource
{
	Texture::Texture(const char* filepath):
		Argent::Resource::ArImportedResource(filepath, ResourceType::Texture)
	{
		descriptor = Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		imDescriptor = Graphics::Graphics::Instance()->GetImGuiHeap()->PopDescriptor();
		const std::filesystem::path path{ filepath };
		const HRESULT hr = Helper::Texture::LoadTexture(Argent::Graphics::Graphics::Instance()->GetDevice(), Argent::Graphics::Graphics::Instance()->GetResourceCmdBundle(), 
		                                                Argent::Graphics::Graphics::Instance()->GetResourceCmdQueue(), path.c_str(), shaderResource.ReleaseAndGetAddressOf());

		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		ID3D12Device* device = Graphics::Graphics::Instance()->GetDevice();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = shaderResource->GetDesc().Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(shaderResource.Get(), &srvDesc,
			descriptor->GetCPUHandle());//ヒープを割り当てる場所

		device->CreateShaderResourceView(shaderResource.Get(), &srvDesc,
			imDescriptor->GetCPUHandle());//ヒープを割り当てる場所

		width = static_cast<float>(shaderResource->GetDesc().Width);
		height = static_cast<float>(shaderResource->GetDesc().Height);
	}

	void Texture::SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT RootParameterIndex) const
	{
		cmdList->SetDescriptorHeaps(1, descriptor->GetDescriptorHeap()->GetHeapDoublePointer());
		cmdList->SetGraphicsRootDescriptorTable(RootParameterIndex, descriptor->GetGPUHandle());
	}
}
