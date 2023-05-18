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

		void SetOffset(const DirectX::XMFLOAT3& f) { offset = f; }
		void SetScale(const DirectX::XMFLOAT3& f) { scale = f; }
		void SetTexPos(const DirectX::XMFLOAT2& f) { texPos = f; }

		DirectX::XMFLOAT3 GetOffset() const { return offset; }
		DirectX::XMFLOAT3 GetScale() const { return scale; }
		DirectX::XMFLOAT2 GetTexPos() const { return texPos; }

		Material::SpriteMaterial* GetMaterial() { return material.get(); }

	private:
		std::unique_ptr<Resource::Sprite::Sprite> sprite;
		std::shared_ptr<Material::SpriteMaterial> material;

		DirectX::XMFLOAT3 offset;
		DirectX::XMFLOAT3 scale{ 1, 1, 1 };
		DirectX::XMFLOAT2 texPos{};
	};
}
