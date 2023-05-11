#include "DirectionalLight.h"

#include "../GameObject/GameObject.h"
#include "../Graphic/Graphics.h"


DirectionalLight::DirectionalLight(int index, std::string name, DirectX::XMFLOAT4 color):
	BaseComponent(name)
,	color(color)
,	index(index)
{
	/*if(Argent::Graphics::Graphics::Instance()->GetLight() == nullptr)
		Argent::Graphics::Graphics::Instance()->SetLight(this);*/
}

void DirectionalLight::Initialize()
{
	auto t = GetOwner()->GetTransform();
	auto p = t->GetPosition();
	p.y = 100;
	p.z = -30;
	t->SetPosition(p);
}

void DirectionalLight::Reset()
{
	color.Reset();
	BaseComponent::Reset();
}

void DirectionalLight::End()
{
	auto g = Argent::Graphics::Graphics::Instance();
	g->SetLightPosition(GetOwner()->GetTransform()->GetPosition(), index);
	g->SetLightColor(GetColor().GetColor(), index);
}

void DirectionalLight::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
	{
		color.DrawDebug();
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}

