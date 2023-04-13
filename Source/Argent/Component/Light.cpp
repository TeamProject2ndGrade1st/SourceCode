#include "Light.h"

#include "../GameObject/GameObject.h"
#include "../Graphic/Graphics.h"


Light::Light(std::string name, DirectX::XMFLOAT4 color):
	BaseComponent(name)
,	color(color)
{
	/*if(Argent::Graphics::ArGraphics::Instance()->GetLight() == nullptr)
		Argent::Graphics::ArGraphics::Instance()->SetLight(this);*/
}

void Light::Reset()
{
	color.Reset();
	BaseComponent::Reset();
}

void Light::End()
{
	auto g = Argent::Graphics::ArGraphics::Instance();
	g->SetLightPosition(GetOwner()->GetTransform()->GetPosition());
	g->SetLightColor(GetColor().GetColor());
}
#ifdef _DEBUG
void Light::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
	{
		color.DrawDebug();
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}
#endif
