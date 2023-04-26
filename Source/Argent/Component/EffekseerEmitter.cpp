#include "EffekseerEmitter.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Other/Helper.h"
#include "../Math/MathHelper.h"


namespace Argent::Component::Renderer
{
	EffekseerEmitter::EffekseerEmitter(const char* filePath, const char* materialPath):
		BaseComponent("Effekseer Emitter")
	{
		effect = std::make_shared<Argent::Resource::Effect::EffekseerResource>(filePath, materialPath);
	}

	void EffekseerEmitter::Update()
	{
		if (!effect) return;

		//todo 毎フレームはいらない気がする
		//再生中だった場合はトランスフォームを毎フレームアップデートする
		if(effect->IsExist())
		{
			const Transform* t = GetOwner()->GetTransform();
			effect->Update(t->GetPosition(), t->GetScale(), t->GetRotation(), color.color);
		}
	}

	void EffekseerEmitter::Render() const
	{
	}

	void EffekseerEmitter::DrawDebug()
	{
		if(ImGui::TreeNode(GetName().c_str()))
		{
			color.DrawDebug();
			BaseComponent::DrawDebug();
			if (ImGui::Button("Play"))
			{
				OnPlay(0.0f);
			}
			ImGui::TreePop();
		}
	}

	void EffekseerEmitter::OnPlay(float startFrame)
	{
		if(!effect) return;

		//エフェクトが再生中だった場合は停止する
		if(effect->IsExist())	effect->Stop();

		//再生
		const auto* t = GetOwner()->GetTransform();
		//スケールファクターに影響されるようにした（なんか出来やん）
		effect->Play(t->GetPosition() + offset, t->GetScale() * t->GetScaleFactor() + scale,
			t->GetRotation() + rotation, startFrame);
	}

	void EffekseerEmitter::Pause()
	{
		if(effect && effect->IsExist())
			effect->Pause();
	}
}
