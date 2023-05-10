#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Descriptor.h"
#include "DescriptorHeap.h"
#include "../../Other/Helper.h"

namespace Argent::Dx12
{
	template<class T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer(ID3D12Device* device, Dx12::Descriptor* descriptor,const T* fillValue = nullptr);
		~ConstantBuffer() = default;

		void UpdateConstantBuffer(const T& t)
		{
			constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&map));
			*map = t;
			//*map = t;
			constantBuffer->Unmap(0, nullptr);
		}

		void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) const
		{
			cmdList->SetDescriptorHeaps(1, descriptor->GetDescriptorHeap()->GetHeapDoublePointer());
			cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex, descriptor->GetGPUHandle());
		}
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;
		Argent::Dx12::Descriptor* descriptor;
		T* map{};
	};

	template <class T>
	ConstantBuffer<T>::ConstantBuffer(ID3D12Device* device, Dx12::Descriptor* descriptor, const T* fillValue):
		descriptor(descriptor)
	{
		HRESULT hr{ S_OK };
		const D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		const CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(Argent::Helper::Math::CalcAlignmentSize(sizeof(T)));

		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(constantBuffer.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		
		hr = constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&map));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr)); 

		if(fillValue)
		{
			*map = *fillValue;
		}

		constantBuffer->Unmap(0, nullptr);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.SizeInBytes = static_cast<UINT>(constantBuffer->GetDesc().Width);
		cbvDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();

		device->CreateConstantBufferView(&cbvDesc, descriptor->GetCPUHandle());

	}
}