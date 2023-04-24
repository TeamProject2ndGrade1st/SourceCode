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

		void DrawDebug() override;

		//エフェクトの再生
		void OnPlay() { isPlay = true; }
	private:
		bool isPlay;
		Argent::Color color;

		bool isLooping;

		std::shared_ptr<Argent::Resource::Effect::EffectResource> effect;
	};
	
}

