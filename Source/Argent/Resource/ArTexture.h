#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "../Graphic/Dx12/Descriptor.h"
#include "ArResource.h"

namespace Argent::Texture
{
	class ArTexture:
		public Argent::Resource::ArImportedResource
	{
	public:
		ArTexture(const char* filePath);

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT RootParameterIndex) const;
		ID3D12Resource* GetTexture() const { return shaderResource.Get(); }
		float GetWidth() const { return width; }
		float GetHeight() const { return height; }
		Dx12::Descriptor* GetDescriptor() const { return descriptor; }
		Dx12::Descriptor* GetImDescriptor() const { return imDescriptor; }

	private:
		Dx12::Descriptor* descriptor;
		Dx12::Descriptor* imDescriptor;
		Microsoft::WRL::ComPtr<ID3D12Resource> shaderResource;
		float width;
		float height;
	};
}
