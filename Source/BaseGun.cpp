#include "BaseGun.h"

BaseGun::BaseGun(const char* name):
	BaseComponent(name)
{
	 
}

void BaseGun::Update()
{
	static float elapsedTime = 0;
	if(Argent::Input::GetButton(MouseButton::LeftButton))
	{
		if(elapsedTime > fireRate)
		{

		}
	}
}
