#include "EffekseerEmitter.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Other/Helper.h"


namespace Argent::Component::Renderer
{
	EffekseerEmitter::EffekseerEmitter(const char* filePath, const char* materialPath):
		BaseComponent(Helper::String::ExtractFileName(filePath, false))
	{
		effect = std::make_shared<Argent::Resource::Effect::EffectResource>(filePath, materialPath);
	}

	void EffekseerEmitter::Update()
	{
		if (!effect) return;

		//todo
		if (!effect->IsExist())
		{
			isPlay = false;
		}
		else
		{
			const Transform* t = GetOwner()->GetTransform();

			if (!t) return;
			effect->Update(t->GetPosition(), t->GetScale(), t->GetRotation(), color.color);
		}
		isPlay = true;
	}

	void EffekseerEmitter::Render() const
	{
		if (!effect) return;
		const Transform* t = GetOwner()->GetTransform();

		if (!t) return;

		if(!effect->IsExist())
		effect->Play(t->GetPosition(), t->GetScale(), t->GetRotation());
	}

	void EffekseerEmitter::DrawDebug()
	{
		if(ImGui::TreeNode(GetName().c_str()))
		{
			color.DrawDebug();
			BaseComponent::DrawDebug();
			ImGui::TreePop();
		}
	}

}
