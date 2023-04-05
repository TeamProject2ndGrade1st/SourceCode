#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include "Descriptor.h"


//�f�B�X�N���v�^�Ǘ��p�̃N���X
//�R���X�g���N�^�̈����Ŋm�ۂ���q�[�v�T�C�Y���w��
//hack�@�f�B�X�N���v�^�q�[�v�Ǘ��p�̃N���X���ق����H
namespace Argent::Descriptor
{

	class ArDescriptorHeap final
	{
	public:
		ArDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors);
		~ArDescriptorHeap() = default;
		ArDescriptorHeap(const ArDescriptorHeap&) = delete;
		ArDescriptorHeap(const ArDescriptorHeap&&) = delete;
		ArDescriptorHeap operator=(const ArDescriptorHeap&) = delete;
		ArDescriptorHeap operator=(const ArDescriptorHeap&&) = delete;

		[[nodiscard]] UINT GetNumDescriptors() const { return MaxDescriptorCounts; }

		/**  // NOLINT(clang-diagnostic-invalid-utf8, clang-diagnostic-invalid-utf8)
			 * \brief �O���珇�ԂɊm�ۂ����f�B�X�N���v�^�[�̃|�C���^�����蓖�ĂĂ��� �m�ۗ̈�𒴂����ꍇ�̓A�T�[�g
			 * \return �f�B�X�N���v�^�[�̃|�C���^��ԋp�@
			 */
		[[nodiscard]] ArDescriptor* PopDescriptor();	
		[[nodiscard]] ID3D12DescriptorHeap* const* GetHeapDoublePointer() const { return descriptorHeap.GetAddressOf(); }
		[[nodiscard]] D3D12_DESCRIPTOR_HEAP_DESC GetDesc() const { return descriptorHeap->GetDesc(); }
		[[nodiscard]] ID3D12DescriptorHeap* GetHeapPointer() const { return descriptorHeap.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
		UINT MaxDescriptorCounts;
		UINT heapIncrementSize;
		UINT descriptorStackCounter;//���݂̃f�B�X�N���v�^�[�������@>= MaxDescriptorCounts �̂Ƃ��ɐV���Ɋ��蓖�Ă悤�Ƃ���ƃA�T�[�g
		std::vector<std::unique_ptr<ArDescriptor>> descriptors;
	public:
	};
}
