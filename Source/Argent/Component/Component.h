#pragma once
#include <DirectXMath.h>
#include <string>

#include "../../../imgui/imgui.h"
class GameObject;
//todo orderInUpdateの追加

//コンポーネントクラスの大元
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

		virtual void Initialize() { Reset(); }	//最初に一回だけ呼ばれる
		virtual void Finalize() {}				//最後に一回だけ呼ばれる


		virtual void Begin() {}					//毎フレーム最初に呼ばれる
		virtual void EarlyUpdate() {}			//Beginのあとに
		virtual void Update() {}				//毎フレームBeginとEndの間で呼ばれる
		virtual void LateUpdate() {}			//updateのあとに
		virtual void Render() const {}				//毎フレーム　描画用関数
		virtual void End() {}					//毎フレーム最後に呼ばれる


		virtual void Reset() {}					//Initializeが呼ばれた直後の状態に戻す

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