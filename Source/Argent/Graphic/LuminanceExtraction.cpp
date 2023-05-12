#include "LuminanceExtraction.h"
#include "Graphics.h"

LuminanceExtraction::LuminanceExtraction(ID3D12Device* device, 
	D3D12_RESOURCE_DESC rsDesc, float clearColor[4])
{
	renderingPipeline = Argent::Graphics::RenderingPipeline::CreateLuminanceExtractionPipeline();

	this->clearColor[0] = clearColor[0];
	this->clearColor[1] = clearColor[1];
	this->clearColor[2] = clearColor[2];
	this->clearColor[3] = clearColor[3];

	HRESULT hr{ S_OK };

	//必要なデスクリプタを取得
	srvDescriptor = Argent::Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
	rtvDescriptor = Argent::Graphics::Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();

	//フォーマットをHDR対応にする
	rsDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(rsDesc.Format, clearColor);

	//シェーダーリソースとしてリソースを作成(バリアの設定をするため最初はシェーダーリソース）
	hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

	//シェーダーリソースとレンダーターゲットそれぞれのビューを作成
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = rsDesc.Format;
		auto handle = rtvDescriptor->GetCPUHandle();
		device->CreateRenderTargetView(resource.Get(), &rtvDesc, handle);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Format = rtvDesc.Format;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		handle = srvDescriptor->GetCPUHandle();
		device->CreateShaderResourceView(resource.Get(), &srvDesc, handle);
	}
	renderingPipeline2 = Argent::Graphics::RenderingPipeline::CreateTmpPipeline();
	resource->SetName(L"GaussianBluerBuffer");

}

void LuminanceExtraction::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, D3D12_VIEWPORT viewport)
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	const auto rtvHandle = rtvDescriptor->GetCPUHandle();

	//リソースの状態をレンダーターゲットに
	cmdList->ResourceBarrier(1, &barrier);

	//レンダーターゲットとしてセット＆クリア
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);

	cmdList->RSSetViewports(1, &viewport);
	cmdList->RSSetScissorRects(1, &rect);
	//
	renderingPipeline->SetOnCommandList(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->IASetVertexBuffers(0, 0, nullptr);
	cmdList->DrawInstanced(4, 1, 0, 0);

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//リソースの状態をシェーダーリソースに戻す
	cmdList->ResourceBarrier(1, &barrier);
}

void LuminanceExtraction::Output(ID3D12GraphicsCommandList* cmdList)
{
	//SetOnCommandList(cmdList, 0);
	renderingPipeline2->SetOnCommandList(cmdList);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	cmdList->DrawInstanced(4, 1, 0, 0);
}


void LuminanceExtraction::SetOnCommandList(ID3D12GraphicsCommandList* cmdList, int rootParameterIndex)
{
	cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex, srvDescriptor->GetGPUHandle());
}

