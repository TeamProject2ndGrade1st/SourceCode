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
		//フォーマットをHDR対応にする
		rsDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

		rtvFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();
		srvFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();

		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(rsDesc.Format, clearColor);

		//シェーダーリソースとしてリソースを作成(バリアの設定をするため最初はシェーダーリソース）
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue, IID_PPV_ARGS(finalResource.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//シェーダーリソースとレンダーターゲットそれぞれのビューを作成
		{
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Format = rsDesc.Format;
			auto handle = rtvFinal->GetCPUHandle();
			device->CreateRenderTargetView(finalResource.Get(), &rtvDesc, handle);

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Format = rtvDesc.Format;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			handle = srvFinal->GetCPUHandle();
			device->CreateShaderResourceView(finalResource.Get(), &srvDesc, handle);
		}

		renderingPipelineFinal = Argent::Graphics::RenderingPipeline::CreateCombineBokeColorPipeline();

		//川瀬式ブルームに必要なダウンサンプリングの変数
		float dev = 1.5;
		UINT64 originalWidth = rsDesc.Width;
		UINT64 originalHeight = rsDesc.Height;

		for(int j = 0; j < 4; ++j)
		{
			originalWidth /= dev;
			for (int i = 0; i < 2; ++i)
			{
				//必要なデスクリプタを取得
				blurResource[j].srvDescriptor[i] = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
				blurResource[j].rtvDescriptor[i] = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();

				

				D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(rsDesc.Format, clearColor);


				//リソースの幅高さを下げていく
				rsDesc.Width = originalWidth;
				rsDesc.Height = originalHeight;

				//シェーダーリソースとしてリソースを作成(バリアの設定をするため最初はシェーダーリソース）
				hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
					&clearValue, IID_PPV_ARGS(blurResource[j].blurResource[i].ReleaseAndGetAddressOf()));
				_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

				//シェーダーリソースとレンダーターゲットそれぞれのビューを作成
				{
					D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
					rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
					rtvDesc.Format = rsDesc.Format;
					auto handle = blurResource[j].rtvDescriptor[i]->GetCPUHandle();
					device->CreateRenderTargetView(blurResource[j].blurResource[i].Get(), &rtvDesc, handle);

					D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
					srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Format = rtvDesc.Format;
					srvDesc.Texture2D.MipLevels = 1;
					srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
					handle = blurResource[j].srvDescriptor[i]->GetCPUHandle();
					device->CreateShaderResourceView(blurResource[j].blurResource[i].Get(), &srvDesc, handle);
				}

				if (i == 0) originalHeight /= dev;
			}
		}

		renderingPipelineX = Argent::Graphics::RenderingPipeline::CreateGaussianBlurPipelineX();
		renderingPipelineY = Argent::Graphics::RenderingPipeline::CreateGaussianBlurPipelineY();

		constantBuffer = std::make_unique<Argent::Dx12::ConstantBuffer<Constant>>(Graphics::Instance()->GetDevice(), 
			Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor(), 
			nullptr);
	}


	void GaussianBlur::SetOnCommandList(ID3D12GraphicsCommandList* cmdList, int rootParameterIndex)
	{
		cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex, srvFinal->GetGPUHandle());
	}

	void GaussianBlur::Execute(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle)
	{
		UpdateWeight(10);
		//renderingPipelineX->SetOnCommandList(cmdList);
		//コンスタントバッファをセット
		constantBuffer->UpdateConstantBuffer(constant);
		//constantBuffer->SetOnCommandList(cmdList, 1);

		
		//1回目のブラー
		Blurred(cmdList, rect, &blurResource[0], srvHandle);
		//2回目のブラー
		Blurred(cmdList, rect, &blurResource[1], blurResource[0].srvDescriptor[1]->GetGPUHandle());
		//3回目のブラー
		Blurred(cmdList, rect, &blurResource[2], blurResource[1].srvDescriptor[1]->GetGPUHandle());
		//4回目のブラー
		Blurred(cmdList, rect, &blurResource[3], blurResource[2].srvDescriptor[1]->GetGPUHandle());

		//すべての平均を取る
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(finalResource.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

		//リソースの状態をレンダーターゲットに
		cmdList->ResourceBarrier(1, &barrier);

		//ビューポートとシザー矩形の大きさを画面サイズに直してセット
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Graphics::Instance()->GetWidth();
		viewport.Height = Graphics::Instance()->GetHeight();
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		cmdList->RSSetViewports(1, &viewport);
		cmdList->RSSetScissorRects(1, &rect);

		auto rtvHandle = rtvFinal->GetCPUHandle();
		cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);
		renderingPipelineFinal->SetOnCommandList(cmdList);
		cmdList->SetGraphicsRootDescriptorTable(0, blurResource[0].srvDescriptor[0]->GetGPUHandle());
		cmdList->SetGraphicsRootDescriptorTable(1, blurResource[1].srvDescriptor[0]->GetGPUHandle());
		cmdList->SetGraphicsRootDescriptorTable(2, blurResource[2].srvDescriptor[0]->GetGPUHandle());
		cmdList->SetGraphicsRootDescriptorTable(3, blurResource[3].srvDescriptor[0]->GetGPUHandle());

		cmdList->DrawInstanced(4, 1, 0, 0);

		barrier = CD3DX12_RESOURCE_BARRIER::Transition(finalResource.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		//リソースの状態をレンダーターゲットに
		cmdList->ResourceBarrier(1, &barrier);
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

	void GaussianBlur::Blurred(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect, 
		BlurBuffer* buffer, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle)
	{
		auto* renderTarget = buffer->blurResource[0].Get();
		auto rtvHandle = buffer->rtvDescriptor[0]->GetCPUHandle();

		//横方向ブラー
		{
			renderingPipelineX->SetOnCommandList(cmdList);
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//リソースの状態をレンダーターゲットに
			cmdList->ResourceBarrier(1, &barrier);


			//レンダーターゲットの大きさに合わせたビューポートとシザー矩形を作成
			D3D12_RESOURCE_DESC rsDesc = renderTarget->GetDesc();
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = rsDesc.Width;
			viewport.Height = rsDesc.Height;
			viewport.MaxDepth = 1.0f;
			viewport.MinDepth = 0.0f;

			D3D12_RECT scissorRect;
			scissorRect.right = rsDesc.Width;
			scissorRect.bottom = rsDesc.Height;
			scissorRect.left = scissorRect.top = 0;

			//ビューポートとシザー矩形
			cmdList->RSSetViewports(1, &viewport);
			cmdList->RSSetScissorRects(1, &scissorRect);

			//レンダーターゲットとしてセット＆クリア
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &scissorRect);


			//シェーダーリソースのセット
			cmdList->SetGraphicsRootDescriptorTable(0, srvHandle);

			//頂点バッファのセット
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//コンスタントバッファのセット
			constantBuffer->SetOnCommandList(cmdList, 1);

			//トポロジのセット
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			//ドローコール
			cmdList->DrawInstanced(4, 1, 0, 0);


			barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//リソースの状態をシェーダーリソースに戻す
			cmdList->ResourceBarrier(1, &barrier);
		}

		//レンダーターゲットとハンドルを切り替える
		renderTarget = buffer->blurResource[1].Get();
		rtvHandle = buffer->rtvDescriptor[1]->GetCPUHandle();
		srvHandle = buffer->srvDescriptor[0]->GetGPUHandle();

		//縦方向ブラー
		{
			//レンダーターゲットの大きさに合わせたビューポートとシザー矩形を作成
			D3D12_RESOURCE_DESC rsDesc = renderTarget->GetDesc();
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = rsDesc.Width;
			viewport.Height = rsDesc.Height;
			viewport.MaxDepth = 1.0f;
			viewport.MinDepth = 0.0f;

			D3D12_RECT scissorRect;
			scissorRect.right = rsDesc.Width;
			scissorRect.bottom = rsDesc.Height;
			scissorRect.left = scissorRect.top = 0;

			//ビューポートとシザー矩形
			cmdList->RSSetViewports(1, &viewport);
			cmdList->RSSetScissorRects(1, &scissorRect);



			renderingPipelineY->SetOnCommandList(cmdList);
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//リソースの状態をレンダーターゲットに
			cmdList->ResourceBarrier(1, &barrier);

			//レンダーターゲットとしてセット＆クリア
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);

			//シェーダーリソースのセット
			cmdList->SetGraphicsRootDescriptorTable(0, srvHandle);

			//頂点バッファのセット
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//コンスタントバッファのセット
			constantBuffer->SetOnCommandList(cmdList, 1);

			//トポロジのセット
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			//ドローコール
			cmdList->DrawInstanced(4, 1, 0, 0);


			barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//リソースの状態をシェーダーリソースに戻す
			cmdList->ResourceBarrier(1, &barrier);
		}
	}
}
