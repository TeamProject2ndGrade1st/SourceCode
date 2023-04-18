#pragma once
#include <memory>
#include "Renderer.h"
#include "../Resource/Sprite.h"

namespace Argent::Component::Renderer
{
	//�X�v���C�g�i�摜�j�`��p�̃N���X
	//todo �`��ɑ���texpos �Ƃ��w��ł��Ȃ��悤�ɂ��Ă邩��X�v���C�g�A�j���[�V�������o���Ȃ�
	class SpriteRenderer final:
		public BaseRenderer
	{

	public:
		SpriteRenderer(const char* filePath);
		~SpriteRenderer() override;


		void Initialize() override;
		void Finalize() override;
		void Begin() override;
		void End() override;
		void Update() override;
		void Render() const override;
		void Render(ID3D12GraphicsCommandList* cmdList) const override;
		

		void DrawDebug() override;

	private:
		std::unique_ptr<Resource::Sprite::Sprite> sprite;
		std::shared_ptr<Material::SpriteMaterial> material;
	};
}
