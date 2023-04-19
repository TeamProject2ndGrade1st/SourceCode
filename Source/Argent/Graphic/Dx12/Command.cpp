#include "Command.h"
#include<d3d12.h>
#include <dxgi1_6.h>

namespace Argent::Dx12
{
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

		cmdList->Close();
	}

	void CommandBundle::Begin(const D3D12_VIEWPORT* viewport, const D3D12_RECT* scissorRect,
		const D3D12_CPU_DESCRIPTOR_HANDLE& dsvHandle, const D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle,
		float clearColor[4]) const
	{
		this->Reset();
		//cmdList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
		//cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		//cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		//cmdList->RSSetViewports(1, viewport);
		//cmdList->RSSetScissorRects(1, scissorRect);
	}

	void CommandBundle::Reset() const
	{
		cmdAlloc.Get()->Reset();
		cmdList.Get()->Reset(cmdAlloc.Get(), nullptr);
	}
}
