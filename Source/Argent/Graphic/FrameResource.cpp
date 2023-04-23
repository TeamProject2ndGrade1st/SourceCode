#include "FrameResource.h"

#include <d3dx12.h>

#include "../Other/Misc.h"
#include "../Other/Helper.h"
#include "Dx12/DescriptorHeap.h"

namespace Argent::Graphics 
{
	FrameResource::FrameResource(ID3D12Device* device, 
	                             IDXGISwapChain3* swapChain, UINT backBufferIndex, 
	                             Dx12::Descriptor* rtv, Dx12::Descriptor* dsv, Dx12::Descriptor* cbv, 
	                             UINT NumCmdLists):
		cbScene(nullptr)
		,	backBufferIndex(backBufferIndex)
		,	rtv(rtv)
		,	dsv(dsv)
		,	cbv(cbv)
	{
		HRESULT hr{ S_OK };
		cmdBundle.resize(NumCmdLists);
		for(UINT i = 0; i < NumCmdLists; ++i)
		{
			cmdBundle.at(i) = std::make_unique<Dx12::CommandBundle>(device);
			hr = swapChain->GetBuffer(backBufferIndex, IID_PPV_ARGS(backBuffer.ReleaseAndGetAddressOf()));
			_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

			device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv->GetCPUHandle());
		}

		//シーン用のコンスタントバッファを作成（カメラの位置とかライトとかその他シーン共通のモノ用)
		const CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(Helper::Math::CalcAlignmentSize(sizeof(SceneConstant)));
		hr = device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
		                                     nullptr, IID_PPV_ARGS(constantBuffer.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));;
		
		hr = constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&cbScene));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		D3D12_CONSTANT_BUFFER_VIEW_DESC cBufferViewDesc{};
		cBufferViewDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();
		cBufferViewDesc.SizeInBytes = static_cast<UINT>(constantBuffer->GetDesc().Width);
		device->CreateConstantBufferView(&cBufferViewDesc, cbv->GetCPUHandle());


		//深度バッファを作る
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.Width = backBuffer->GetDesc().Width;
		resDesc.Height = backBuffer->GetDesc().Height;
		resDesc.DepthOrArraySize = 1;
		resDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		resDesc.SampleDesc.Count = 1;
		resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			
		//超重要！！　クリアバリュー
		D3D12_CLEAR_VALUE depthClearValue{};
		depthClearValue.DepthStencil.Depth = 1.0f;
		depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		                                     &depthClearValue, IID_PPV_ARGS(depthStencilResource.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));;
			

		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.NumDescriptors = 1;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		D3D12_DEPTH_STENCIL_VIEW_DESC dsView{};
		dsView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsView.Flags = D3D12_DSV_FLAG_NONE;
		dsView.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		device->CreateDepthStencilView(depthStencilResource.Get(), &dsView, dsv->GetCPUHandle());

		backBuffer->SetName(L"BackBuffer");
		constantBuffer->SetName(L"ConstantBuffer");
		depthStencilResource->SetName(L"DepthStencilResource");
	}

	void FrameResource::UpdateSceneConstant(const SceneConstant& sceneConstant) const
	{
		*cbScene = sceneConstant;
	}


	void FrameResource::SetSceneConstant(UINT rootParameterIndex)
	{
		//cmdBundle.at(0)->cmdList->SetDescriptorHeaps(1, cbv->GetDescriptorHeap()->GetHeapDoublePointer());
		cmdBundle.at(static_cast<int>(RenderType::Mesh))->cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex, cbv->GetGPUHandle());
	}

	void FrameResource::SetBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const
	{
		D3D12_RESOURCE_BARRIER barrier{};
		
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.StateBefore = before;
		barrier.Transition.StateAfter = after;
		barrier.Transition.Subresource = 0;
		barrier.Transition.pResource = backBuffer.Get();
		
		cmdBundle[static_cast<int>(RenderType::PostRendering)]->cmdList->ResourceBarrier(1, &barrier);
	}

	void FrameResource::Reset()
	{
		for(auto& bundle : cmdBundle)
		{
			bundle.get()->Reset();
		}
	}

	void FrameResource::Terminate()
	{
		for(auto& bundle : cmdBundle)
		{
			bundle.get()->Close();
		}
		this->Reset();
	}

	void FrameResource::WaitForEvent(Dx12::CommandQueue* cmdQueue)
	{
		if(cmdQueue->fence->GetCompletedValue() < fenceValue)
		{
			HANDLE fenceEvent{};
			cmdQueue->fence->SetEventOnCompletion(fenceValue, fenceEvent);
			WaitForSingleObject(fenceEvent, INFINITE);
		}
	}


	void FrameResource::Begin() const
	{
		for(auto& bundle : cmdBundle)
		{
			bundle->Begin();
		}
	}

	void FrameResource::End() const
	{
		for(const auto& bundle : cmdBundle)
		{
			bundle->cmdList->Close();
		}
	}

	void FrameResource::SetRenderTarget(const D3D12_VIEWPORT& viewport, const D3D12_RECT& rect, float clearColor[4])
	{
		//auto dsvHandle = gfx->GetDepthHandle();
		//todo
		const auto dsvHandle = dsv->GetCPUHandle();
		//const auto dsvHandle = gfx->GetCurrentFrameResource()->GetDsv()->GetCPUHandle();
		const auto rtvHandle = rtv->GetCPUHandle();

		GetCmdList(RenderType::PostRendering)->OMSetRenderTargets(1, &rtvHandle,
			false, &dsvHandle);
		SetBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		GetCmdList(RenderType::PostRendering)->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		GetCmdList(RenderType::PostRendering)->ClearRenderTargetView(rtv->GetCPUHandle(), clearColor, 0, nullptr);
		GetCmdList(RenderType::PostRendering)->RSSetViewports(1, &viewport);
		GetCmdList(RenderType::PostRendering)->RSSetScissorRects(1, &rect);
	}
}
