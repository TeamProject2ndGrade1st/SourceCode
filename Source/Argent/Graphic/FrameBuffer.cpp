#include "FrameBuffer.h"

#include <cassert>
#include "d3dx12.h"
#include "Graphics.h"
#include "FrameResource.h"

namespace Argent::Graphics
{
	FrameBuffer::FrameBuffer(ID3D12Device* device, D3D12_RESOURCE_DESC rsDesc,
		float clearColor[4])
	{
		this->clearColor[0] = clearColor[0];
		this->clearColor[1] = clearColor[1];
		this->clearColor[2] = clearColor[2];
		this->clearColor[3] = clearColor[3];

		HRESULT hr{ S_OK };

		//必要なデスクリプタを取得
		srvDescriptor = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		rtvDescriptor = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();
		dsvDescriptor = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV)->PopDescriptor();


		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
		//シェーダーリソースとしてリソースを作成(バリアの設定をするため最初はシェーダーリソース）
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//シェーダーリソースとレンダーターゲットそれぞれのビューを作成
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		auto handle = rtvDescriptor->GetCPUHandle();
		device->CreateRenderTargetView(resource.Get(), &rtvDesc, handle);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Format = rtvDesc.Format;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		handle = srvDescriptor->GetCPUHandle();
		device->CreateShaderResourceView(resource.Get(), &srvDesc, handle);

		//深度バッファを作る
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.Width = rsDesc.Width;
		resDesc.Height = rsDesc.Height;
		resDesc.DepthOrArraySize = 1;
		resDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		resDesc.SampleDesc.Count = 1;
		resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			
		heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			
		//超重要！！　クリアバリュー
		D3D12_CLEAR_VALUE depthClearValue{};
		depthClearValue.DepthStencil.Depth = 1.0f;
		depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		                                     &depthClearValue, IID_PPV_ARGS(depthResource.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));;
			

		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.NumDescriptors = 1;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		D3D12_DEPTH_STENCIL_VIEW_DESC dsView{};
		dsView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsView.Flags = D3D12_DSV_FLAG_NONE;
		dsView.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		device->CreateDepthStencilView(depthResource.Get(), &dsView, dsvDescriptor->GetCPUHandle());



		//頂点情報を入れる(スクリーンサイズの長方形)
		std::vector<Vertex> vertice
		{
			{{ -1.f,  1.f, 0.0f },{ 0.0f, 0.0f }},
			{{  1.f,  1.f, 0.0f },{ 1.0f, 0.0f }},
			{{ -1.f, -1.f, 0.0f },{ 0.0f, 1.0f }},
			{{  1.f, -1.f, 0.0f },{ 1.0f, 1.0f }},
		};

		vertexBuffer = std::make_unique<Dx12::ArVertexBuffer<Vertex>>(device, vertice);
		renderingPipeline = RenderingPipeline::CreateDefaultFrameBufferPipeline();
	}

	void FrameBuffer::Begin(const Graphics* gfx, ID3D12GraphicsCommandList* cList)
	{
		cmdList = cList;

		const auto dsvHandle = dsvDescriptor->GetCPUHandle();
		const auto rtvHandle = rtvDescriptor->GetCPUHandle();

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			resource.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &barrier);

		cmdList->OMSetRenderTargets(1, &rtvHandle,
			false, &dsvHandle);

		//auto dsvHandle = gfx->GetDepthHandle();
		//todo
		const D3D12_VIEWPORT viewport = gfx->GetViewport();
		const D3D12_RECT rect = gfx->GetRect();


		cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		cmdList->ClearRenderTargetView(rtvDescriptor->GetCPUHandle(), clearColor, 0, nullptr);
		cmdList->RSSetViewports(1, &viewport);
		cmdList->RSSetScissorRects(1, &rect);
	}

	void FrameBuffer::End(const Graphics* gfx) const
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			resource.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		cmdList->ResourceBarrier(1, &barrier);
		//gfx->GetCommandList(RenderType::Sprite)->ResourceBarrier(1, &barrier);
	}

	void FrameBuffer::Draw(const Graphics* gfx) const
	{
		ID3D12GraphicsCommandList* commandList = gfx->GetCommandList(RenderType::PostRendering);
		renderingPipeline->SetOnCommandList(commandList);
		commandList->SetGraphicsRootDescriptorTable(0, srvDescriptor->GetGPUHandle());

		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		vertexBuffer->SetOnCommandList(commandList, 0);
		commandList->DrawInstanced(4, 1, 0, 0);
	}
}