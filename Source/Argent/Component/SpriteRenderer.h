#pragma once
#include <memory>
#include "Renderer.h"
#include "../Resource/Sprite.h"

namespace Argent::Component::Renderer
{
	//スプライト（画像）描画用のクラス
	//todo 描画に多分texpos とか指定できないようにしてるからスプライトアニメーションが出来ない
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
