#include "Stage.h"
#include "Argent/Component/RayCast.h"
Stage::Stage():
	BaseComponent("Stage")
{}

void Stage::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/map_0419_1.fbx"));
	auto r = GetOwner()->GetComponent<Argent::Component::Renderer::MeshRenderer>();
	GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(r->GetMesh()->meshResource));
}
