#include "Descriptor.h"
#include "DescriptorHeap.h"

namespace Argent::Dx12
{
	Descriptor::Descriptor(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
	                           UINT heapIndex, DescriptorHeap* heapPointer):
		cpuHandle(cpuHandle)
		,	gpuHandle(gpuHandle)
		,	heapIndex(heapIndex)
		,	heapPointer(heapPointer)
	{}
}
