#pragma once
#include "Argent/Argent.h"


class BaseGun:
	public Argent::Component::BaseComponent
{
public:
	BaseGun(const char* name);
	virtual ~BaseGun() override = default;

	template<class T>
	void Shot(DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& direction);
	//virtual void Recoil() = 0;
protected:
	
};

template <class T>
void BaseGun::Shot(DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& direction)
{
	T* bulletActor = new T(direction, 0, 0.1f);
	GameObject::Instantiate("bullet", bulletActor);
}

