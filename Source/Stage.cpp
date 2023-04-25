#include "Stage.h"
#include "Argent/Component/RayCast.h"
Stage::Stage():
	BaseComponent("Stage")
{}

void Stage::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/map_0419_1.fbx"));
	std::vector<Argent::Resource::Mesh::MeshResource> mResource;
	auto r = GetOwner()->GetComponent<Argent::Component::Renderer::MeshRenderer>();
	if(r)
	{
		mResource.emplace_back(r->GetMesh()->meshResource);
	}

	for(auto it = GetOwner()->begin(); it != GetOwner()->end(); ++it)
	{
		if(!(*it)) return;
		auto r = (*it)->GetComponent<Argent::Component::Renderer::MeshRenderer>();
		if(r)
		{
			mResource.emplace_back(r->GetMesh()->meshResource);
		}
	}

	GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(mResource));

	GetOwner()->SetTag(GameObject::Tag::Stage);
	//GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));

	//GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(r->GetMesh()->meshResource));
	//GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
}
