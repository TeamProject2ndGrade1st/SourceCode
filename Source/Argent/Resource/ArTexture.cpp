#include "ArTexture.h"

#include "ArResource.h"
#include "../Graphic/Graphics.h"
#include "../Graphic/Dx12/DescriptorHeap.h"
#include "../Other/Helper.h"
#include "../Resource/ResourceManager.h"

namespace Argent::Texture
{
	ArTexture::ArTexture(const char* filepath):
		Argent::Resource::ArImportedResource(filepath, ResourceType::Texture)
	{
		descriptor = Graphics::ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		imDescriptor = Graphics::ArGraphics::Instance()->GetImGuiHeap()->PopDescriptor();
		const std::filesystem::path path{ filepath };
		const HRESULT hr = Helper::Texture::LoadTexture(Argent::Graphics::ArGraphics::Instance()->GetDevice(), Argent::Graphics::ArGraphics::Instance()->GetResourceCmdBundle(), 
		                                                Argent::Graphics::ArGraphics::Instance()->GetResourceCmdQueue(), path.c_str(), shaderResource.ReleaseAndGetAddressOf());

		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		ID3D12Device* device = Graphics::ArGraphics::Instance()->GetDevice();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = shaderResource->GetDesc().Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(shaderResource.Get(), &srvDesc,
			descriptor->GetCPUHandle());//�q�[�v�����蓖�Ă�ꏊ

		device->CreateShaderResourceView(shaderResource.Get(), &srvDesc,
			imDescriptor->GetCPUHandle());//�q�[�v�����蓖�Ă�ꏊ

		width = static_cast<float>(shaderResource->GetDesc().Width);
		height = static_cast<float>(shaderResource->GetDesc().Height);
	}

	void ArTexture::Render(ID3D12GraphicsCommandList* cmdList, UINT RootParameterIndex) const
	{
		cmdList->SetDescriptorHeaps(1, descriptor->GetDescriptorHeap()->GetHeapDoublePointer());
		cmdList->SetGraphicsRootDescriptorTable(RootParameterIndex, descriptor->GetGPUHandle());
	}
}
