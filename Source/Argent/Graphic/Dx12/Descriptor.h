#pragma once
#include <d3d12.h>

namespace Argent::Dx12
{
	class DescriptorHeap;
	class Descriptor
	{
	public:
		Descriptor(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, UINT heapIndex, DescriptorHeap* heapPointer);
		~Descriptor() = default;
		Descriptor(const Descriptor&) = delete;
		Descriptor(const Descriptor&&) = delete;
		Descriptor operator=(const Descriptor&) = delete;
		Descriptor operator=(const Descriptor&&) = delete;


		[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return gpuHandle; }
		[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const { return cpuHandle; }
		[[nodiscard]] DescriptorHeap* GetDescriptorHeap() const { return heapPointer; }

		UINT GetHeapIndex() const { return heapIndex; }

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
		UINT heapIndex;
		DescriptorHeap* heapPointer;
	};
}
