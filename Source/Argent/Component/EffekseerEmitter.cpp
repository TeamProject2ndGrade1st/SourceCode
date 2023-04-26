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

		//todo ���t���[���͂���Ȃ��C������
		//�Đ����������ꍇ�̓g�����X�t�H�[���𖈃t���[���A�b�v�f�[�g����
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

		//�G�t�F�N�g���Đ����������ꍇ�͒�~����
		if(effect->IsExist())	effect->Stop();

		//�Đ�
		const auto* t = GetOwner()->GetTransform();
		//�X�P�[���t�@�N�^�[�ɉe�������悤�ɂ����i�Ȃ񂩏o�����j
		effect->Play(t->GetPosition() + offset, t->GetScale() * t->GetScaleFactor() + scale,
			t->GetRotation() + rotation, startFrame);
	}

	void EffekseerEmitter::Pause()
	{
		if(effect && effect->IsExist())
			effect->Pause();
	}
}
