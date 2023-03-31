#include "BaseActor.h"
#include "../GameObject/GameObject.h"


namespace Argent::Component
{
	BaseActor::BaseActor(const char* name):
		ArComponent(name)
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
