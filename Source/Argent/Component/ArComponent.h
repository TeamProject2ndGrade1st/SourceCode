#pragma once
#include <DirectXMath.h>
#include <string>

#include "../../../imgui/imgui.h"
class GameObject;
//todo orderInUpdate�̒ǉ�

//�R���|�[�l���g�N���X�̑匳
namespace Argent::Component
{
	class ArComponent
	{
	public:
		ArComponent(const std::string& n) :
			owner(nullptr)
		{
			name = n;
		}
		virtual  ~ArComponent() = default;
		ArComponent(const ArComponent& com) = default;
		ArComponent(const ArComponent&& com) noexcept;

		GameObject* GetOwner() const { return owner; }
		void SetOwner(GameObject* p) { owner = p; }

		virtual void Initialize() { Reset(); }	//�ŏ��Ɉ�񂾂��Ă΂��
		virtual void Finalize() {}				//�Ō�Ɉ�񂾂��Ă΂��
		virtual void Begin() {}					//���t���[���ŏ��ɌĂ΂��
		virtual void End() {}					//���t���[���Ō�ɌĂ΂��
		virtual void Update() {}				//���t���[��Begin��End�̊ԂŌĂ΂��
		virtual void Render() const {}				//���t���[���@�`��p�֐�
		virtual void Reset() {}					//Initialize���Ă΂ꂽ����̏�Ԃɖ߂�
#ifdef _DEBUG
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
#endif

	public:
		std::string GetName() const { return name; }

	private:
		GameObject* owner;
		std::string name;
		//std::vector<std::unordered_map<char[], std::set<char>>>* unti;
	};

}