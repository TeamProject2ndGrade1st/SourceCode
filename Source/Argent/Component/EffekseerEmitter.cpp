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
		scale = DirectX::XMFLOAT3(1, 1, 1);
	}

	void EffekseerEmitter::Update()
	{
		if (!effect) return;

		//todo ���t���[���͂���Ȃ��C������
		//�Đ����������ꍇ�̓g�����X�t�H�[���𖈃t���[���A�b�v�f�[�g����
		if(effect->IsExist())
		{
			const Transform* t = GetOwner()->GetTransform();
			effect->Update(t->GetPosition() + offset, t->GetScale() * scale, ToRadians(t->GetRotation()) + ToRadians(rotation), color.color);
		}
	}

	void EffekseerEmitter::Render() const
	{
	}

	void EffekseerEmitter::Finalize()
	{
		effect->Stop();
	}

	void EffekseerEmitter::DrawDebug()
	{
		if(ImGui::TreeNode(GetName()))
		{
			ImGui::DragFloat3("Offset", &offset.x, 0.5f, -FLT_MAX, FLT_MAX);
			ImGui::DragFloat3("Scale", &scale.x, 0.1f, -FLT_MAX, FLT_MAX);
			ImGui::DragFloat4("Rotation", &rotation.x, 1.0f, -FLT_MAX, FLT_MAX);
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
		effect->Play(t->GetPosition() + offset, t->GetScale() * scale, ToRadians(t->GetRotation()) + ToRadians(rotation), startFrame);
	}

	void EffekseerEmitter::Pause()
	{
		if(effect && effect->IsExist())
			effect->Pause();
	}
}
