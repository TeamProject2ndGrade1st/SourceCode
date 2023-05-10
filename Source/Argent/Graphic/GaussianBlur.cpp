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

		//�K�v�ȃf�X�N���v�^���擾
		srvDescriptorX = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		rtvDescriptorX = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();
		srvDescriptorFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->PopDescriptor();
		rtvDescriptorFinal = Graphics::Instance()->GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->PopDescriptor();

		//�t�H�[�}�b�g��HDR�Ή��ɂ���
		rsDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(rsDesc.Format, clearColor);

		//�V�F�[�_�[���\�[�X�Ƃ��ă��\�[�X���쐬(�o���A�̐ݒ�����邽�ߍŏ��̓V�F�[�_�[���\�[�X�j
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue, IID_PPV_ARGS(blurResourceX.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue, IID_PPV_ARGS(finalBlurResource.ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//�V�F�[�_�[���\�[�X�ƃ����_�[�^�[�Q�b�g���ꂼ��̃r���[���쐬
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
		//�R���X�^���g�o�b�t�@���Z�b�g
		constantBuffer->UpdateConstantBuffer(constant);
		constantBuffer->SetOnCommandList(cmdList, 1);

		//�������̃u���[
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(blurResourceX.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			const auto rtvHandle = rtvDescriptorX->GetCPUHandle();

			//���\�[�X�̏�Ԃ������_�[�^�[�Q�b�g��
			cmdList->ResourceBarrier(1, &barrier);

			//�����_�[�^�[�Q�b�g�Ƃ��ăZ�b�g���N���A
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);

			//�p�C�v���C�����Z�b�g
			renderingPipelineX->SetOnCommandList(cmdList);

			//���_�o�b�t�@�̃Z�b�g
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//�g�|���W�̃Z�b�g
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			//�h���[�R�[��
			cmdList->DrawInstanced(4, 1, 0, 0);

			barrier = CD3DX12_RESOURCE_BARRIER::Transition(blurResourceX.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//���\�[�X�̏�Ԃ��V�F�[�_�[���\�[�X�ɖ߂�
			cmdList->ResourceBarrier(1, &barrier);
		}

		//���u���[�����e�N�X�`�����Z�b�g
		cmdList->SetGraphicsRootDescriptorTable(0, srvDescriptorX->GetGPUHandle());

		//�c�����̃u���[
		{
			renderingPipelineY->SetOnCommandList(cmdList);
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(finalBlurResource.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			const auto rtvHandle = rtvDescriptorFinal->GetCPUHandle();

			//���\�[�X�̏�Ԃ������_�[�^�[�Q�b�g��
			cmdList->ResourceBarrier(1, &barrier);

			//�����_�[�^�[�Q�b�g�Ƃ��ăZ�b�g���N���A
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
			cmdList->ClearRenderTargetView(rtvHandle, clearColor, 1, &rect);

			//�p�C�v���C�����Z�b�g
			renderingPipelineX->SetOnCommandList(cmdList);

			//���_�o�b�t�@�̃Z�b�g
			cmdList->IASetVertexBuffers(0, 0, nullptr);

			//�g�|���W�̃Z�b�g
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			//�h���[�R�[��
			cmdList->DrawInstanced(4, 1, 0, 0);



			barrier = CD3DX12_RESOURCE_BARRIER::Transition(finalBlurResource.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			//���\�[�X�̏�Ԃ��V�F�[�_�[���\�[�X�ɖ߂�
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
		// �K�i��
		for (int i = 0; i < NumWeight; i++) 
		{
			constant.weight[i] /= total;
		}
	}
}
