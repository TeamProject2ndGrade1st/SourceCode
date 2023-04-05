#pragma once
#include "Component.h"
#include "../Resource/Effect.h"
#include "Color.h"

namespace Argent::Component::Renderer
{
	class EffectRenderer:
		public ArComponent
	{
	public:
		EffectRenderer(const char* filePath, const char* materialPath);
		~EffectRenderer() override = default;

		void Update() override;
		void Render() const override;
#ifdef _DEBUG
		void DrawDebug() override;
#endif
		//エフェクトの再生
		void OnPlay() { isPlay = true; }
	private:
		bool isPlay;
		Argent::ArColor color;

		std::shared_ptr<Argent::Resource::Effect::ArEffect> effect;
	};
	
}

