// ReSharper disable All
#include "Transform.h"

#include "../GameObject/GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "../Math/MathHelper.h"

Transform& Transform::operator+=(const Transform& t)
{
	this->position.x += t.position.x;
	this->position.y += t.position.y;
	this->position.z += t.position.z;

	this->rotation.x += t.rotation.x;
	this->rotation.y += t.rotation.y;
	this->rotation.z += t.rotation.z;
	return *this;
}

Transform& Transform::operator=(const Transform& t)
{
	this->position = t.position;
	this->scale = t.scale;
	this->rotation = t.rotation;

	this->scaleFactor = t.scaleFactor;
	this->coordinateSystem = t.coordinateSystem;
	return *this;
}

#ifdef _DEBUG
void Transform::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
	{
		ImGui::DragFloat3("Position", &position.x, 1.0f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat3("Scale", &scale.x, 0.001f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat3("Rotation", &rotation.x, 0.5f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("ScaleFactor", &scaleFactor, 0.001f, 0.001f, 1.0f);
		ImGui::SliderInt("Coordinate System", &coordinateSystem, 0, static_cast<int>(CoordinateSystem::cLeftZup));

		ImGui::Text(coordinateSystemName[coordinateSystem].c_str());


		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}
#endif

void Transform::Reset()
{
	if(GetOwner()->GetComponent<Camera>() || GetOwner()->GetComponent<Light>())
	{
		position = DirectX::XMFLOAT3(0, 0, -10);
	}
	else 
	{
		position = DirectX::XMFLOAT3();
	}
	scale = DirectX::XMFLOAT3(1, 1, 1);
	rotation = DirectX::XMFLOAT4(0, 0, 0, 0);
}

void Transform::SetWorld(const DirectX::XMFLOAT4X4& w)
{
	defaultWorld = w;
}

DirectX::XMMATRIX Transform::GetMatrix() const
{
	const DirectX::XMMATRIX C = { DirectX::XMLoadFloat4x4(&CoordinateSystemTransforms[coordinateSystem]) *
		DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
	const DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) };
	const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z)) };
	const DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x,position.y, position.z) };
	const DirectX::XMMATRIX Dw = DirectX::XMLoadFloat4x4(&defaultWorld);
	return  Dw * (C * S * R * T);
}

Transform Transform::AdjustParentTransform() const
{
	Transform tmp = *this;
	//tmp += *this;
	if(GetOwner())
	{
		if(GetOwner()->GetParent())
		{
			const Transform t = GetOwner()->GetParent()->GetTransform()->AdjustParentTransform();
			tmp += t;
			tmp.scale = tmp.scale * t.scale;
			tmp.scaleFactor *= t.GetScaleFactor();
			tmp.coordinateSystem = t.coordinateSystem;
		}
	}
	return tmp;
}

void Transform::SetTransform(const Transform& t)
{
	*this = t; 
}

DirectX::XMFLOAT3 Transform::CalcForward()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMFLOAT3 ret{};
	DirectX::XMStoreFloat3(&ret, DirectX::XMVector3Normalize(rotationMatrix.r[2]));
	return ret;
}
