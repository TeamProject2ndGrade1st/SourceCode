#pragma once
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>

#include "../Component/Component.h"
#include "../Component/Transform.h"
#include "../Component/BaseActor.h"


class GameObject
{
public:
	enum class Tag : unsigned int
	{
		UnTagged   =	0x01 << 1,
		MainCamera =	0x01 << 2,
		Stage =			0x01 << 3,
		Friend =		0x01 << 4,
		SpikeBot =		0x01 << 5,
		Turret =		0x01 << 6,
		FriendManager = 0x01 << 7
	};
	GameObject(std::string name = "gameObject", Argent::Component::BaseComponent* c = nullptr);
	GameObject(std::string name, std::vector<Argent::Component::BaseComponent*> com);
	GameObject(std::initializer_list<Argent::Component::BaseComponent*> components, std::string name = "gameObject");

	virtual ~GameObject() = default;
	GameObject(const GameObject&) = delete;
	GameObject(const GameObject&&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject& operator=(const GameObject&&) = delete;

	virtual void Initialize();
	virtual void Finalize();

	virtual void Begin();
	virtual void EarlyUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render() const;
	virtual void End();



	virtual void DrawDebug();

	void AddComponent(Argent::Component::BaseComponent* com);
	void AddComponent(std::vector<Argent::Component::BaseComponent*> com);
	void AddChild(GameObject* obj);

	const std::string& GetName() const				{ return name; }
	int GetOrderInUpdate() const { return orderInUpdate;  }
	GameObject* GetParent() const					{ return parent; }
	template <typename T> T* GetChild();
	template <typename T> T* GetComponent();
	Transform* GetTransform() const
	{
		return transform;
	}
	Argent::Component::BaseActor* GetActor() const	{ return actor; }

	bool GetIsSelected() const { return isSelected; }
	int GetChildCount() const { return static_cast<int>(childObjects.size()); }
	bool GetIsActive() const { return isActive; }
	bool GetDestroyFlag() const { return willDestroy; }

	void SetName(std::string s) { name = s; }
	void SetOrderInUpdate(int i) { orderInUpdate = i;  }
	void SetParent(GameObject* p) { parent = p; }
	void SetActor(Argent::Component::BaseActor* a) { actor = a;  }
	void SetActive(bool b)
	{
		isActive = b;
		if(!isInitialized)
			Initialize();
	}
	

	void SetIsSelected(bool b) { isSelected = b; }

	std::vector<GameObject*>::iterator begin() { return childObjects.begin(); }
	std::vector<GameObject*>::iterator end() { return childObjects.end(); }

	void ReverseIsSelected() { isSelected = !isSelected; }

	void CloseWindow() { isSelected = false; }
	void CloseAllWindow();


	Tag GetTag() const { return tag; }
	void SetTag(Tag t) { tag = t; }


	/**
	 * \brief ゲームオブジェクトを破壊する
	 * \param object 破壊したいゲームオブジェクト
	 */
	static void Destroy(GameObject* object);
	/**
	 * \brief ゲームオブジェクトの生成
	 * \param name オブジェクトの名称
	 * \param com 追加したいコンポーネント
	 * \return 生成したオブジェクト
	 */
	static GameObject* Instantiate(const char* name, Argent::Component::BaseComponent* com);
	/**
	 * \brief 最初に見つけたオブジェクトのみ戻す
	 * \param name 探したいオブジェクトの名前
	 * \return 
	 */
	static GameObject* FindByName(const char* name);
	/**
	 * \brief 特定のタグを持ったゲームオブジェクトの配列を返す
	 *ビット演算で見つけるため2つ以上のタグを組み合わせることも可能なはず…
	 * \param tag 
	 * \param objArray [out] 発見したゲームオブジェクトを入れる配列
	 * \return 発見したかどうか
	 */
	static bool FindByTag(Tag tag, std::vector<GameObject*>& objArray);

	//todo もっといい方法を考える
	float elapsedTimeFromDestroyed;
protected:
	std::string name;
	std::vector<Argent::Component::BaseComponent*> components;
	std::vector<GameObject*> childObjects;
	GameObject* parent;
	Transform* transform;

	Argent::Component::BaseActor* actor{};

	int orderInUpdate = 10;

	//std::string tag = "UnTagged";
	Tag tag;

	bool isSelected;
	bool isInitialized;
	bool willDestroy;
	bool isActive;

public:
	bool operator<(const GameObject& g) const
	{
		return this->orderInUpdate < g.orderInUpdate;
	}
};

template <typename T>
T* GameObject::GetComponent()
{
	for(auto& com : components)
	{
		if(typeid(*com) != typeid(T)) continue;

		return static_cast<T*>(com);
	}
	return nullptr;
}

template <typename T>
T* GameObject::GetChild()
{
	for(auto& obj : childObjects)
	{
		if(typeid(*obj) != typeid(T)) continue;

		return static_cast<T*>(obj);
	}
	return nullptr;
}
