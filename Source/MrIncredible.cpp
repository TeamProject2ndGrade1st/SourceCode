#include "MrIncredible.h"
#include "Argent/GameObject/GameObject.h"


MrIncredible::MrIncredible():
	Argent::Component::ArComponent("MrIncredible")
{
}

void MrIncredible::Initialize()
{
	ArComponent::Initialize();
	GetOwner()->GetTransform()->SetScaleFactor(0.01);
}
