#pragma once
#include <DirectXMath.h>
#include "ArMesh.h"


namespace Argent::Resource
{
	namespace Sprite
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texcoord;
			DirectX::XMFLOAT4 color;
		};

		//�X�v���C�g�`��Ɏg�����_�o�b�t�@�𒸓_���������Ă�N���X
		//hack�@�����Ƀe�N�X�`����center �Ƃ�texpos�������Ă����������c

		class Sprite:
			public Mesh::ArMesh<Vertex>
		{
		public:
			Sprite();

			/*void SetOnCommandList(ID3D12GraphicsCommandList* cmdList) override
			{
				ArMesh::SetOnCommandList(cmdList, 0, 1);
				DrawCall(cmdList, static_cast<UINT>(indices.size()));
			}*/
			/**
				 * \brief ���_���̍X�V�p
				 * \param position ���W(Z�l�͖��������)
				 * \param scale �X�P�[��(Z�l�͖��������)
				 * \param center ��]�̒��S�n
				 * \param angle ��]��
				 * \param textureWidth �e�N�X�`���̕�
				 * \param textureHeight �e�N�X�`���̍���
				 * \param color �F
				 */
			void UpdateVertexMap(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT2& center, float angle, float textureWidth, float textureHeight, const DirectX::XMFLOAT4& color) const;

		private:
			Vertex* vertexMap{};
		};
	}

}

