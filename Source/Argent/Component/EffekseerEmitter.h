#pragma once
#include "Component.h"
#include "../Resource/Effect.h"
#include "Color.h"

namespace Argent::Component::Renderer
{
	class EffekseerEmitter:
		public BaseComponent
	{
	public:
		EffekseerEmitter(const char* filePath, const char* materialPath);
		~EffekseerEmitter() override = default;

		void Update() override;
		void Render() const override;
		void Finalize() override;
		void DrawDebug() override;

		//エフェクトの再生
		void OnPlay(float startFrame = 0.0f);
		void Pause();
	public:

		Argent::Color color;
		DirectX::XMFLOAT3 offset;
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT4 rotation;
		bool isLooping;
		std::shared_ptr<Argent::Resource::Effect::EffekseerResource> effect;
	};
	
}

