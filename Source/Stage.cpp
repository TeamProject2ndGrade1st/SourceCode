#include "Stage.h"
#include "Argent/Component/RayCast.h"
Stage::Stage(const char* filePath):
	BaseComponent("Stage")
,	filePath(filePath)
{}

void Stage::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx(filePath.c_str()));

	std::vector<Argent::Resource::Mesh::MeshResource> mResource;
	Argent::Component::Renderer::MeshRenderer* r = GetOwner()->GetComponent<Argent::Component::Renderer::MeshRenderer>();
	if(r)
	{
		mResource.emplace_back(r->GetMesh()->meshResource);
	}

	for(auto it = GetOwner()->begin(); it != GetOwner()->end(); ++it)
	{
		if(!(*it)) continue;
		auto r = (*it)->GetComponent<Argent::Component::Renderer::MeshRenderer>();
		if(r)
		{
			mResource.emplace_back(r->GetMesh()->meshResource);
		}
	}

	GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(mResource));

	GetOwner()->ReplaceTag(GameObject::Tag::Stage);
	GetOwner()->GetTransform()->SetScaleFactor(0.2f);
}
