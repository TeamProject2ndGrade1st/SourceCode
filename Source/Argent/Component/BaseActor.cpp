#include "BaseActor.h"
#include "../GameObject/GameObject.h"


namespace Argent::Component
{
	BaseActor::BaseActor(const char* name):
		BaseComponent(name)
	{
		GameObject* obj = GetOwner();
		if(obj)
			GetOwner()->SetActor(this);
	}

	void BaseActor::Initialize()
	{
		GetOwner()->SetActor(this);
	}

	Transform* BaseActor::GetTransform() const
	{
		GameObject* obj = GetOwner();
		if(!obj) _ASSERT_EXPR(false, "Missing Owner");
		Transform* ret = obj->GetTransform();
		if(!obj) _ASSERT_EXPR(false, "Missing Transform Component");
		return ret;
	}
	
}
