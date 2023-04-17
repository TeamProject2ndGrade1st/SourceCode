#include "DescriptorHeap.h"
#include "d3dx12.h"
#include "../../Other/Misc.h"

namespace Argent::Dx12
{
	DescriptorHeap::DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors):
		MaxDescriptorCounts(numDescriptors)
		,	heapIncrementSize(device->GetDescriptorHandleIncrementSize(type))
		,	descriptorStackCounter(0)
	{
		LPCWSTR str{};
		D3D12_DESCRIPTOR_HEAP_FLAGS descHeapFlag = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		switch (type)
		{
		case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
			str = L"CBV_SRV_UAV DescriptorHeap";
			descHeapFlag = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
			str = L"Sampler DescriptorHeap";
			descHeapFlag = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
			str = L"DSV DescriptorHeap";
			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
			str = L"RTV DescriptorHeap";
			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES:
			_ASSERT_EXPR(FALSE, L"Parameter is different");
			break;
		}

		HRESULT hr{ S_OK };
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Flags = descHeapFlag;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Type = type;

		hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		descriptorHeap->SetName(str);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuH = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		D3D12_GPU_DESCRIPTOR_HANDLE gpuH{};
		if(type != D3D12_DESCRIPTOR_HEAP_TYPE_DSV && type != D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		{
			//Shader visible �ł͂Ȃ��f�X�N���v�^�ł͌Ă�ł͂����Ȃ��֐��炵��
			gpuH = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}
			

		UINT heapIndex = 0;

		//�ő吔�̊m��
		descriptors.resize(numDescriptors);
		for(auto& desc : descriptors)
		{
			desc = std::make_unique<Dx12::Descriptor>(cpuH, gpuH, heapIndex, this);
			cpuH.ptr += heapIncrementSize;
			if (type != D3D12_DESCRIPTOR_HEAP_TYPE_DSV && type != D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
				gpuH.ptr += heapIncrementSize;
			++heapIndex;
		}
	}

	Descriptor* DescriptorHeap::PopDescriptor()
	{
		if(descriptorStackCounter >= descriptors.size()) _ASSERT_EXPR(FALSE, L"Overed heapSize");
		return descriptors.at(descriptorStackCounter++).get();
	}
}
