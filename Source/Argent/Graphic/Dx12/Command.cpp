#include "Command.h"
#include<d3d12.h>
#include <dxgi1_6.h>
#include "../FrameResource.h"

namespace Argent::Dx12
{
	void CommandQueue::SetFence(UINT numCmdList, Graphics::FrameResource* resource)
	{
		if(!resource)
		{
			cmdQueue->Signal(fence.Get(), ++fenceValue);
		}
		else
		{
			resource->fenceValue += 1;
			fenceValue = resource->fenceValue;
			cmdQueue->Signal(fence.Get(), resource->fenceValue);
		}
	}

	CommandBundle::CommandBundle(ID3D12Device* device)
	{
		HRESULT hr{ S_OK };

		hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, 
		                                    IID_PPV_ARGS(cmdAlloc.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		                               cmdAlloc.Get(), nullptr, 
		                               IID_PPV_ARGS(cmdList.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		executeCmdLists = cmdList.Get();

		cmdList->SetName(L"CmdLists");
		cmdAlloc->SetName(L"CmdAlloc");
		executeCmdLists->SetName(L"ExeCmdLists");
		isClosed = false;
		Close();
		this->Reset();
	}


	void CommandBundle::Begin()
	{
		Close();
		this->Reset();
	}

	void CommandBundle::Reset()
	{
		if (!isClosed) return;
		cmdAlloc.Get()->Reset();
		cmdList.Get()->Reset(cmdAlloc.Get(), nullptr);
		isClosed = false;
	}
}
