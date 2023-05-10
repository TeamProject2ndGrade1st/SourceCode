#include "GaussianBlur.h"
#include "Graphics.h"

#include "d3dx12.h"

namespace Argent::Graphics
{
	GaussianBlur::GaussianBlur(ID3D12Device* device, D3D12_RESOURCE_DESC rsDesc, float clearColor[4])
	{
		this->clearColor[0] = clearColor[0];
		this->clearColor[1] = clearColor[1];
		this->clearColor[2] = clearColor[2];
		this->clearColor[3] = clearColor[3];

		HRESULT hr{ S_OK };

		//必要なデスクリプタを取得
		srvDescriptorX = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		rtvDescriptorX = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();
		srvDescriptorFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		rtvDescriptorFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();

		//フォーマットをHDR対応にする
		rsDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(rsDesc.Format, clearColor);

		//シェーダーリソースとしてリソースを作成(バリアの設定をするため最初はシェーダーリソース）
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue, IID_PPV_ARGS(blurResourceX.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue, IID_PPV_ARGS(finalBlurResource.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//シェーダーリソースとレンダーターゲットそれぞれのビューを作成
		{
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Format = rsDesc.Format;
			auto handle = rtvDescriptorX->GetCPUHandle();
			device->CreateRenderTargetView(blurResourceX.Get(), &rtvDesc, handle);
			handle = rtvDescriptorFinal->GetCPUHandle();
			device->CreateRenderTargetView(finalBlurResource.Get(), &rtvDesc, handle);


			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Format = rtvDesc.Format;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			handle = srvDescriptorX->GetCPUHandle();
			device->CreateShaderResourceView(blurResourceX.Get(), &srvDesc, handle);
			handle = srvDescriptorFinal->GetCPUHandle();
			device->CreateShaderResourceView(finalBlurResource.Get(), &srvDesc, handle);
		}
		
		blurResourceX->SetName(L"GaussianBluerBufferX");
		finalBlurResource->SetName(L"GaussianBluerBufferY");


		renderingPipelineX = Argent::Graphics::RenderingPipeline::CreateGaussianBlurPipelineX();
		renderingPipelineY = Argent::Graphics::RenderingPipeline::CreateGaussianBlurPipelineY();

		constantBuffer = std::make_unique<Argent::Dx12::ConstantBuffer<Constant>>(Graphics::Instance()->GetDevice(), 
			Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor(), 
			nullptr);
	}


	void GaussianBlur::SetOnCommandList(ID3D12GraphicsCommandList* cmdList, int rootParameterIndex)
	{
		cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex, srvDescriptorFinal->GetGPUHandle());
	}

	void GaussianBlur::Execute(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect)
	{
		UpdateWeight(20);
		renderingPipelineX->SetOnCommandList(cmdList);
		//コンスタントバッファをセット
		constantBuffer->UpdateConstantBuffer(constant);
		constantBuffer->SetOnCommandList(cmdList, 1);

		//横方向のブラー
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(blurResourceX.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			const auto rtvHandle = rtvDescriptorX->GetCPUHandle();

			//リソースの状態をレンダーターゲットに
			cmdList->ResourceBarrier(1, &barrier);

			//レンダーターゲットとしてセット＆クリア
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);

			//パイプラインをセット
			renderingPipelineX->SetOnCommandList(cmdList);

			//頂点バッファのセット
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//トポロジのセット
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			//ドローコール
			cmdList->DrawInstanced(4, 1, 0, 0);

			barrier = CD3DX12_RESOURCE_BARRIER::Transition(blurResourceX.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//リソースの状態をシェーダーリソースに戻す
			cmdList->ResourceBarrier(1, &barrier);
		}

		//横ブラーしたテクスチャをセット
		cmdList->SetGraphicsRootDescriptorTable(0, srvDescriptorX->GetGPUHandle());

		//縦方向のブラー
		{
			renderingPipelineY->SetOnCommandList(cmdList);
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(finalBlurResource.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			const auto rtvHandle = rtvDescriptorFinal->GetCPUHandle();

			//リソースの状態をレンダーターゲットに
			cmdList->ResourceBarrier(1, &barrier);

			//レンダーターゲットとしてセット＆クリア
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);

			//パイプラインをセット
			renderingPipelineX->SetOnCommandList(cmdList);

			//頂点バッファのセット
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//トポロジのセット
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			//ドローコール
			cmdList->DrawInstanced(4, 1, 0, 0);



			barrier = CD3DX12_RESOURCE_BARRIER::Transition(finalBlurResource.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//リソースの状態をシェーダーリソースに戻す
			cmdList->ResourceBarrier(1, &barrier);
		}

		cmdList->SetGraphicsRootDescriptorTable(0, srvDescriptorX->GetGPUHandle());
		cmdList->SetGraphicsRootDescriptorTable(1, srvDescriptorFinal->GetGPUHandle());
	}

	void GaussianBlur::UpdateWeight(float power)
	{
		float total = 0;
		for (int i = 0; i < NumWeight; i++)
		{
			constant.weight[i] = expf(-0.5f * static_cast<float>(i * i) / power);
			total += 2.0f * constant.weight[i];
		}
		// 規格化
		for (int i = 0; i < NumWeight; i++) 
		{
			constant.weight[i] /= total;
		}
	}
}
