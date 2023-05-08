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

		UnTagged   =	0x01 << 1,	//default 
		MainCamera =	0x01 << 2,	//�J�����������Ă�

		Light =			0x01 << 3,
		Stage =			0x01 << 4,	//�X�e�[�W
		Friend =		0x01 << 5,	//�����L����
		Enemy =			0x01 << 6,	//�G�L����
		EnemyManager =	0x01 << 7,
		FriendManager = 0x01 << 8,
		Core =			0x01 << 9,
		Creature =		0x01 << 10,	
		Machine =		0x01 << 11,
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
	unsigned GetUnsignedTag() const { return static_cast<unsigned>(tag); }
	void ReplaceTag(Tag t) { tag = t; }
	void AddTag(Tag t)
	{
		tag = static_cast<Tag>(static_cast<unsigned>(tag) | static_cast<unsigned>(t));
	}

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
	static GameObject* Instantiate(const char* name, std::vector<Argent::Component::BaseComponent*> com);

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
	 * \return �����������ǂ��� true �����@false ���ĂȂ�
	 */
	static bool FindByTag(Tag tag, std::vector<GameObject*>& objArray);

	/**
	 * \brief child�̔z����擾
	 * \param array child������
	 * \return �����������ǂ����@true�����@false ���ĂȂ�
	 */
	bool GetChildArray(std::vector<GameObject*>& array);
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
	bool isFinInitialized = false;
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

	std::string ComponentNameCheck(const char* name, int num = 0, bool isChecked = false)
	{
		std::string replaceName = name;
		for(const auto& object : components)
		{
			if (!object) continue;
			if(object->GetName() == replaceName)
			{
				if(isChecked)
				{
					replaceName.erase(replaceName.find('('));
				}
				std::string n = replaceName + "(" + std::to_string(num) + ")";
				++num;
				replaceName = ComponentNameCheck(n.c_str(), num, true);
				int i = 0;
			}
		}
		return replaceName;
	}
};

template <typename T>
T* GameObject::GetComponent()
{
	for(auto& com : components)
	{
		if(!com.get()) continue;
		if(typeid(*com.get()) != typeid(T)) continue;

		return static_cast<T*>(com.get());
	}
	return nullptr;
}

