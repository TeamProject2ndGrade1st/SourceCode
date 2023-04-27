#include "GunRenderer.h"

void GunRenderer::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/LMG/LMG_low_0324_1.fbx"""));

}
