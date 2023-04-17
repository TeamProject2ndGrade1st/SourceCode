#include "Light.h"

#include "../GameObject/GameObject.h"
#include "../Graphic/Graphics.h"


Light::Light(std::string name, DirectX::XMFLOAT4 color):
	BaseComponent(name)
,	color(color)
{
	/*if(Argent::Graphics::Graphics::Instance()->GetLight() == nullptr)
		Argent::Graphics::Graphics::Instance()->SetLight(this);*/
}

void Light::Reset()
{
	color.Reset();
	BaseComponent::Reset();
}

void Light::End()
{
	auto g = Argent::Graphics::Graphics::Instance();
	g->SetLightPosition(GetOwner()->GetTransform()->GetPosition());
	g->SetLightColor(GetColor().GetColor());
}

void Light::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
	{
		color.DrawDebug();
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}

