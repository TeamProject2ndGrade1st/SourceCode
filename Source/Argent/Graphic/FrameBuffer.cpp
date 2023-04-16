#include "FrameBuffer.h"

#include <cassert>
#include "d3dx12.h"
#include "Graphics.h"

namespace Argent::Graphics
{
	FrameBuffer::FrameBuffer(ID3D12Device* device, D3D12_RESOURCE_DESC rDesc,
		float clearColor[4],
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc)
	{
		HRESULT hr{ S_OK };

		//レンダーターゲットとシェーダリソース用のデスクリプタを取得
		srvDescriptor = ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		rtvDescriptor = ArGraphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();

		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
		this->clearColor[0] = clearColor[0];
		this->clearColor[1] = clearColor[1];
		this->clearColor[2] = clearColor[2];
		this->clearColor[3] = clearColor[3];
		//シェーダーリソースとしてリソースを作成(バリアの設定をするため最初はシェーダーリソース）
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
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

	void FrameBuffer::Begin(const ArGraphics* gfx) const
	{
		ID3D12GraphicsCommandList* cmdList = gfx->GetCommandList();
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			resource.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &barrier);

		//auto dsvHandle = gfx->GetDepthHandle();
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = gfx->GetCurrentFrameResource()->GetDsv()->GetCPUHandle();
		//todo
		auto handle = rtvDescriptor->GetCPUHandle();

		cmdList->OMSetRenderTargets(1, &handle,
			false, &dsvHandle);
		cmdList->ClearRenderTargetView(rtvDescriptor->GetCPUHandle(), clearColor, 0, nullptr);


		D3D12_VIEWPORT viewport = gfx->GetViewport();
		D3D12_RECT rect = gfx->GetRect();
		cmdList->RSSetViewports(1, &viewport);
		cmdList->RSSetScissorRects(1, &rect);
	}

	void FrameBuffer::End(const ArGraphics* gfx) const
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			resource.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		gfx->GetCommandList()->ResourceBarrier(1, &barrier);
	}

	void FrameBuffer::Draw(const ArGraphics* gfx) const
	{
		ID3D12GraphicsCommandList* cmdList = gfx->GetCommandList();
		//cmdList->SetPipelineState(pipeline.Get());
		//cmdList->SetGraphicsRootSignature(rootSignature.Get());
		renderingPipeline->SetOnCommandList(cmdList);
		cmdList->SetGraphicsRootDescriptorTable(0, srvDescriptor->GetGPUHandle());

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		vertexBuffer->SetOnCommandList(cmdList, 0);
		cmdList->DrawInstanced(4, 1, 0, 0);
	}
}