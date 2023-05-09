#include "Light.h"

#include "../GameObject/GameObject.h"
#include "../Graphic/Graphics.h"
#include "../Component/RayCast.h"


DirectionalLight::DirectionalLight(int index, std::string name, DirectX::XMFLOAT3 color,
	float power):
	BaseComponent(name)
,	color(color)
,	power(power)
,	index(index)
,	direction(DirectX::XMFLOAT3(0, -1.0f, 1.0f))
{}

void DirectionalLight::Reset()
{
	BaseComponent::Reset();
}

void DirectionalLight::Render() const
{
	auto g = Argent::Graphics::Graphics::Instance();
	Argent::Graphics::DirectionalLight directionalLight{};
	directionalLight.direction = DirectX::XMFLOAT4(direction.x, direction.y, direction.z, 0);
	directionalLight.color = DirectX::XMFLOAT4(color.x, color.y, color.z, power);
	g->SetDirectionalLight(directionalLight);
}

void DirectionalLight::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
	{
		ImGui::DragFloat3("Direction", &direction.x, 0.1f, -FLT_MAX, FLT_MAX);
		ImGui::ColorPicker3("Color", &color.x);
		ImGui::DragFloat("Power", &power, 0.1f, 0, FLT_MAX);
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}

void PointLight::Initialize()
{
#ifdef _DEBUG
	GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider);
#endif
}

void PointLight::Render() const
{
	auto g = Argent::Graphics::Graphics::Instance();
	Argent::Graphics::PointLight pointLight{};
	pointLight.position = GetOwner()->GetTransform()->GetPosition();
	pointLight.range = range;
	pointLight.color = DirectX::XMFLOAT4(color.x, color.y, color.z, power);
	g->SetPointLight(pointLight, index);
}

void PointLight::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
	{
		ImGui::DragFloat("Range", &range, 0.1f, -FLT_MAX, FLT_MAX);
		ImGui::ColorPicker3("Color", &color.x);
		ImGui::DragFloat("Power", &power, 0.1f, 0, FLT_MAX);
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}

