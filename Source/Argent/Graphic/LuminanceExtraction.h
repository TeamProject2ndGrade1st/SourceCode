#pragma once
#include "RenderingPipeline.h"
#include "Dx12/Descriptor.h"

class LuminanceExtraction
{
public:
	LuminanceExtraction(ID3D12Device* device, 
	D3D12_RESOURCE_DESC rsDesc, float clearColor[4]);

	//luminance�̃o�b�t�@�ɍ��P�x��������������
	/**
	 * \brief 
	 * \param cmdList 
	 * \param rect 
	 * \param gpuHandle ���P�x�����𒊏o�������e�N�X�`���̃n���h��
	 */
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_RECT rect,
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, D3D12_VIEWPORT viewport);

	void Output(ID3D12GraphicsCommandList* cmdList);
	void SetOnCommandList(ID3D12GraphicsCommandList* cmdList, int rootParameterIndex);
private:
	std::unique_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline;
	std::unique_ptr<Argent::Graphics::RenderingPipeline> renderingPipeline2;	//�ꎞ�I�Ȃ���
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;	//���P�x�������o�̌��ʂ��i�[����o�b�t�@
	Argent::Dx12::Descriptor* rtvDescriptor;
	Argent::Dx12::Descriptor* srvDescriptor;
	float clearColor[4];
};

