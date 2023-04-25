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
	};
	GameObject(std::string name = "gameObject", Argent::Component::BaseComponent* c = nullptr);
	GameObject(std::string name, std::vector<Argent::Component::BaseComponent*> com);
	GameObject(std::initializer_list<Argent::Component::BaseComponent*> coms, std::string name = "gameObject");

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
	template <typename T> T* GetComponent();
	Transform* GetTransform() const
	{
		return transform;
	}
	Argent::Component::BaseActor* GetActor() const	{ return actor; }

	bool GetIsSelected() const { return isDrawDebug; }
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
	

	void SetIsDrawDebug(bool b) { isDrawDebug = b; }

	std::vector<std::unique_ptr<GameObject>>::iterator begin() { return childObjects.begin(); }
	std::vector<std::unique_ptr<GameObject>>::iterator end() { return childObjects.end(); }

	void ReverseIsDrawDebug() { isDrawDebug = !isDrawDebug; }

	void CloseWindow() { isDrawDebug = false; }
	void CloseAllWindow();


	Tag GetTag() const { return tag; }
	void SetTag(Tag t) { tag = t; }


	/**
	 * \brief �Q�[���I�u�W�F�N�g��j�󂷂�
	 * \param object �j�󂵂����Q�[���I�u�W�F�N�g
	 */
	static void Destroy(GameObject* object);
	/**
	 * \brief �Q�[���I�u�W�F�N�g�̐���
	 * \param name �I�u�W�F�N�g�̖���
	 * \param com �ǉ��������R���|�[�l���g
	 * \return ���������I�u�W�F�N�g
	 */
	static GameObject* Instantiate(const char* name, Argent::Component::BaseComponent* com);
	/**
	 * \brief �ŏ��Ɍ������I�u�W�F�N�g�̂ݖ߂�
	 * \param name �T�������I�u�W�F�N�g�̖��O
	 * \return 
	 */
	static GameObject* FindByName(const char* name);
	/**
	 * \brief ����̃^�O���������Q�[���I�u�W�F�N�g�̔z���Ԃ�
	 *�r�b�g���Z�Ō����邽��2�ȏ�̃^�O��g�ݍ��킹�邱�Ƃ��\�Ȃ͂��c
	 * \param tag 
	 * \param objArray [out] ���������Q�[���I�u�W�F�N�g������z��
	 * \return �����������ǂ���
	 */
	static bool FindByTag(Tag tag, std::vector<GameObject*>& objArray);

	//todo �����Ƃ������@���l����
	float elapsedTimeFromDestroyed;
protected:
	std::string name;
	std::vector<std::unique_ptr<Argent::Component::BaseComponent>> components;
	std::vector<std::unique_ptr<GameObject>> childObjects;
	GameObject* parent;
	Transform* transform;

	Argent::Component::BaseActor* actor{};

	int orderInUpdate = 10;

	Tag tag;

	bool isDrawDebug;
	bool isInitialized;
	bool willDestroy;
	bool isActive;

public:
	bool operator<(const GameObject& g) const
	{
		return this->orderInUpdate < g.orderInUpdate;
	}

private:
	int64_t FindNullChildIndex() const;
	int64_t FindNullComponentIndex() const;
};

template <typename T>
T* GameObject::GetComponent()
{
	for(auto& com : components)
	{
		if(typeid(*com.get()) != typeid(T)) continue;

		return static_cast<T*>(com.get());
	}
	return nullptr;
}

