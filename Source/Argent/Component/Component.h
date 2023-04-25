#pragma once
#include <DirectXMath.h>
#include <string>

#include "../../../imgui/imgui.h"
class GameObject;
//todo orderInUpdate�̒ǉ�

//�R���|�[�l���g�N���X�̑匳
namespace Argent::Component
{
	class BaseComponent
	{
	public:
		BaseComponent(const std::string& n) :
			owner(nullptr)
		{
			name = n;
		}
		virtual  ~BaseComponent() = default;
		BaseComponent(const BaseComponent& com) = default;
		BaseComponent(const BaseComponent&& com) noexcept;

		GameObject* GetOwner() const { return owner; }
		void SetOwner(GameObject* p) { owner = p; }

		virtual void Initialize() { Reset(); }	//�ŏ��Ɉ�񂾂��Ă΂��
		virtual void Finalize() {}				//�Ō�Ɉ�񂾂��Ă΂��


		virtual void Begin() {}					//���t���[���ŏ��ɌĂ΂��
		virtual void EarlyUpdate() {}			//Begin�̂��Ƃ�
		virtual void Update() {}				//���t���[��Begin��End�̊ԂŌĂ΂��
		virtual void LateUpdate() {}			//update�̂��Ƃ�
		virtual void Render() const {}				//���t���[���@�`��p�֐�
		virtual void End() {}					//���t���[���Ō�ɌĂ΂��


		virtual void Reset() {}					//Initialize���Ă΂ꂽ����̏�Ԃɖ߂�

		virtual void DrawDebug()
		{
			DebugReset();
		}

		void DebugReset()
		{
			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}

		const char* GetName() const { return name.c_str(); }
		void SetName(const char* n){ name = n; }
	private:
		std::string name;
		GameObject* owner;
	};

}