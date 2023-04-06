#include "EffectRenderer.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Other/Helper.h"


namespace Argent::Component::Renderer
{
	EffectRenderer::EffectRenderer(const char* filePath, const char* materialPath):
		BaseComponent(Helper::String::ExtractFileName(filePath, false))
	{
		effect = std::make_shared<Argent::Resource::Effect::EffectResource>(filePath, materialPath);
	}

	void EffectRenderer::Update()
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

	void EffectRenderer::Render() const
	{
		if (!effect) return;
		const Transform* t = GetOwner()->GetTransform();

		if (!t) return;

		if(!effect->IsExist())
		effect->Play(t->GetPosition(), t->GetScale(), t->GetRotation());
	}

#ifdef _DEBUG
	void EffectRenderer::DrawDebug()
	{
		if(ImGui::TreeNode(GetName().c_str()))
		{
			color.DrawDebug();
			BaseComponent::DrawDebug();
			ImGui::TreePop();
		}
	}
#endif
}
