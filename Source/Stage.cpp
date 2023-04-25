#include "Stage.h"
#include "Argent/Component/RayCast.h"


void Stage::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx(filePath.c_str()));

	std::vector<Argent::Resource::Mesh::MeshResource> mResource;
	Argent::Component::Renderer::MeshRenderer* r = GetOwner()->GetComponent<Argent::Component::Renderer::MeshRenderer>();
	if(r)
	{
		mResource.emplace_back(r->GetMesh()->meshResource);
	}

	GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(mResource));

	GetOwner()->ReplaceTag(GameObject::Tag::Stage);
	GetOwner()->GetTransform()->SetScaleFactor(0.2f);
}

void Box::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx(filePath));
	GetOwner()->ReplaceTag(GameObject::Tag::Stage);
}

void OwnCamp::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx(filePath));
	GetOwner()->ReplaceTag(GameObject::Tag::Stage);
}

void Core::Initialize()
{
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx(filePath));
	auto t = GetOwner()->GetTransform();
	t->SetPosition(DirectX::XMFLOAT3(0, 0, 320));
	t->SetScaleFactor(0.2f);
}

void Core::Update()
{
}
