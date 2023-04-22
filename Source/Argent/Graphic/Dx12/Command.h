#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include "../../Other/Misc.h"

namespace Argent::Graphics
{
	class FrameResource;
}

namespace Argent::Dx12
{
	struct CommandQueue
	{
		CommandQueue(ID3D12Device* device):
			cmdQueue(nullptr)
			,	fence(nullptr)
			,	fenceEvent()
			,	fenceValue(NULL)
		{
			HRESULT hr{ S_OK };
			D3D12_COMMAND_QUEUE_DESC desc{};
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 0;
			desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			hr = device->CreateCommandQueue(&desc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf()));
			_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

			hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
			_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
			cmdQueue->SetName(L"CommandQueue");
			fence->SetName(L"Fence");
		}
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	cmdQueue;
		Microsoft::WRL::ComPtr<ID3D12Fence>		fence;
		HANDLE						fenceEvent;
		uint64_t					fenceValue;

		void SetFence(UINT numCmdList = 1, Graphics::FrameResource* resource = nullptr);
		

		void WaitForFence(int numCount = 1)
		{
			if(fence->GetCompletedValue() < fenceValue)
			{
				fence->SetEventOnCompletion(fenceValue, fenceEvent);
				WaitForMultipleObjects( numCount, &fenceEvent, true, INFINITE);
			}
		}

		void Execute(UINT NumCommandLists, ID3D12CommandList* const* cmdLists) const
		{
			cmdQueue->ExecuteCommandLists(NumCommandLists, cmdLists);
		}
	};

	struct CommandBundle
	{
		CommandBundle(ID3D12Device* device);
		void Begin(const D3D12_VIEWPORT* viewport, const D3D12_RECT* scissorRect,
			const D3D12_CPU_DESCRIPTOR_HANDLE& dsvHandle, const D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle,
			float clearColor[4]) const;
		void Reset() const;
		void Close()
		{
			if(!isClosed)
			{
				cmdList->Close();
				isClosed = true;
			}
		}

		bool isClosed;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAlloc{};
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList{};
		Microsoft::WRL::ComPtr<ID3D12CommandList> executeCmdLists{};
	};
}
