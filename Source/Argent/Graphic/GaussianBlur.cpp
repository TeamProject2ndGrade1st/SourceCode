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
		//�t�H�[�}�b�g��HDR�Ή��ɂ���
		rsDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

		rtvFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();
		srvFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();

		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(rsDesc.Format, clearColor);

		//�V�F�[�_�[���\�[�X�Ƃ��ă��\�[�X���쐬(�o���A�̐ݒ�����邽�ߍŏ��̓V�F�[�_�[���\�[�X�j
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue, IID_PPV_ARGS(finalResource.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//�V�F�[�_�[���\�[�X�ƃ����_�[�^�[�Q�b�g���ꂼ��̃r���[���쐬
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

		//�쐣���u���[���ɕK�v�ȃ_�E���T���v�����O�̕ϐ�
		float dev = 1.5;
		UINT64 originalWidth = rsDesc.Width;
		UINT64 originalHeight = rsDesc.Height;

		for(int j = 0; j < 4; ++j)
		{
			originalWidth /= dev;
			for (int i = 0; i < 2; ++i)
			{
				//�K�v�ȃf�X�N���v�^���擾
				blurResource[j].srvDescriptor[i] = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
				blurResource[j].rtvDescriptor[i] = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();

				

				D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(rsDesc.Format, clearColor);


				//���\�[�X�̕������������Ă���
				rsDesc.Width = originalWidth;
				rsDesc.Height = originalHeight;

				//�V�F�[�_�[���\�[�X�Ƃ��ă��\�[�X���쐬(�o���A�̐ݒ�����邽�ߍŏ��̓V�F�[�_�[���\�[�X�j
				hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
					&clearValue, IID_PPV_ARGS(blurResource[j].blurResource[i].ReleaseAndGetAddressOf()));
				_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

				//�V�F�[�_�[���\�[�X�ƃ����_�[�^�[�Q�b�g���ꂼ��̃r���[���쐬
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
		//�R���X�^���g�o�b�t�@���Z�b�g
		constantBuffer->UpdateConstantBuffer(constant);
		//constantBuffer->SetOnCommandList(cmdList, 1);

		
		//1��ڂ̃u���[
		Blurred(cmdList, rect, &blurResource[0], srvHandle);
		//2��ڂ̃u���[
		Blurred(cmdList, rect, &blurResource[1], blurResource[0].srvDescriptor[1]->GetGPUHandle());
		//3��ڂ̃u���[
		Blurred(cmdList, rect, &blurResource[2], blurResource[1].srvDescriptor[1]->GetGPUHandle());
		//4��ڂ̃u���[
		Blurred(cmdList, rect, &blurResource[3], blurResource[2].srvDescriptor[1]->GetGPUHandle());

		//���ׂĂ̕��ς����
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(finalResource.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

		//���\�[�X�̏�Ԃ������_�[�^�[�Q�b�g��
		cmdList->ResourceBarrier(1, &barrier);

		//�r���[�|�[�g�ƃV�U�[��`�̑傫������ʃT�C�Y�ɒ����ăZ�b�g
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

		//���\�[�X�̏�Ԃ������_�[�^�[�Q�b�g��
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
		// �K�i��
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

		//�������u���[
		{
			renderingPipelineX->SetOnCommandList(cmdList);
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//���\�[�X�̏�Ԃ������_�[�^�[�Q�b�g��
			cmdList->ResourceBarrier(1, &barrier);


			//�����_�[�^�[�Q�b�g�̑傫���ɍ��킹���r���[�|�[�g�ƃV�U�[��`���쐬
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

			//�r���[�|�[�g�ƃV�U�[��`
			cmdList->RSSetViewports(1, &viewport);
			cmdList->RSSetScissorRects(1, &scissorRect);

			//�����_�[�^�[�Q�b�g�Ƃ��ăZ�b�g���N���A
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &scissorRect);


			//�V�F�[�_�[���\�[�X�̃Z�b�g
			cmdList->SetGraphicsRootDescriptorTable(0, srvHandle);

			//���_�o�b�t�@�̃Z�b�g
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//�R���X�^���g�o�b�t�@�̃Z�b�g
			constantBuffer->SetOnCommandList(cmdList, 1);

			//�g�|���W�̃Z�b�g
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			//�h���[�R�[��
			cmdList->DrawInstanced(4, 1, 0, 0);


			barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//���\�[�X�̏�Ԃ��V�F�[�_�[���\�[�X�ɖ߂�
			cmdList->ResourceBarrier(1, &barrier);
		}

		//�����_�[�^�[�Q�b�g�ƃn���h����؂�ւ���
		renderTarget = buffer->blurResource[1].Get();
		rtvHandle = buffer->rtvDescriptor[1]->GetCPUHandle();
		srvHandle = buffer->srvDescriptor[0]->GetGPUHandle();

		//�c�����u���[
		{
			//�����_�[�^�[�Q�b�g�̑傫���ɍ��킹���r���[�|�[�g�ƃV�U�[��`���쐬
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

			//�r���[�|�[�g�ƃV�U�[��`
			cmdList->RSSetViewports(1, &viewport);
			cmdList->RSSetScissorRects(1, &scissorRect);



			renderingPipelineY->SetOnCommandList(cmdList);
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//���\�[�X�̏�Ԃ������_�[�^�[�Q�b�g��
			cmdList->ResourceBarrier(1, &barrier);

			//�����_�[�^�[�Q�b�g�Ƃ��ăZ�b�g���N���A
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);

			//�V�F�[�_�[���\�[�X�̃Z�b�g
			cmdList->SetGraphicsRootDescriptorTable(0, srvHandle);

			//���_�o�b�t�@�̃Z�b�g
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//�R���X�^���g�o�b�t�@�̃Z�b�g
			constantBuffer->SetOnCommandList(cmdList, 1);

			//�g�|���W�̃Z�b�g
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			//�h���[�R�[��
			cmdList->DrawInstanced(4, 1, 0, 0);


			barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget,
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//���\�[�X�̏�Ԃ��V�F�[�_�[���\�[�X�ɖ߂�
			cmdList->ResourceBarrier(1, &barrier);
		}
	}
}
