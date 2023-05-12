#include "Grenade.h"

void Grenade::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/pack_0424_3.fbx"));
}
