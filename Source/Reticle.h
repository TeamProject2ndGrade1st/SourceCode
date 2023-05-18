#pragma once
#include "Argent/Argent.h"


class Reticle:
	public Argent::Component::BaseComponent
{
public:
	Reticle(const char* filePath):
		BaseComponent("Reticle")
	,	filePath(filePath)
	{}

	void Initialize() override;
private:
	const char* filePath;
};

inline void Reticle::Initialize()
{
	auto g = GetOwner();
	auto t = g->GetTransform();
	t->SetPosition(DirectX::XMFLOAT3(640, 360, 0));
	t->SetScale(DirectX::XMFLOAT3(0.4f, 0.4f, 0));
	auto com = new Argent::Component::Renderer::SpriteRenderer(filePath);
	com->SetTexPos(DirectX::XMFLOAT2(50.0f, 50.0f));
	GetOwner()->AddComponent(com);
}
