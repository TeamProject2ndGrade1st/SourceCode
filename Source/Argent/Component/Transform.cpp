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

Transform Transform::operator=(const Transform& t)
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
	position = DirectX::XMFLOAT3(w.m[3][0], w.m[3][1], w.m[3][2]);
	const DirectX::XMFLOAT3 sX = DirectX::XMFLOAT3(w.m[0][0], w.m[0][1], w.m[0][2]);
	DirectX::XMStoreFloat(&scale.x, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&sX)));
	const DirectX::XMFLOAT3 sY = DirectX::XMFLOAT3(w.m[1][0], w.m[1][1], w.m[1][2]);
	DirectX::XMStoreFloat(&scale.y, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&sY)));
	const DirectX::XMFLOAT3 sZ = DirectX::XMFLOAT3(w.m[2][0], w.m[2][1], w.m[2][2]);
	DirectX::XMStoreFloat(&scale.z, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&sZ)));
	//	scale = DirectX::XMFLOAT3(w.m[0][0], w.m[1][1], w.m[2][2]);

	float angleX, angleY, angleZ;


	//ワールド行列からローカル座標の向きをそれぞれ抽出
	//DirectX::XMFLOAT3 axisX = DirectX::XMFLOAT3(w.m[0][0], w.m[0][1], w.m[0][2]);
	//DirectX::XMFLOAT3 axisY = DirectX::XMFLOAT3(w.m[1][0], w.m[1][1], w.m[1][2]);
	//DirectX::XMFLOAT3 axisZ = DirectX::XMFLOAT3(w.m[2][0], w.m[2][1], w.m[2][2]);

	//DirectX::XMFLOAT4X4 world = w;
	//world.m[3][0] =
	//	world.m[3][1] =
	//	world.m[3][2] = 0.0f;
	//world.m[3][3] = 1.0f;

	//DirectX::XMStoreFloat4x4(&world, DirectX::XMLoadFloat4x4(&world) * DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)));

	angleY = asinf(w.m[0][2]);
	//cos(angleY) == 0
	if(fabsf(cos(angleY)) < 0.01f)
	{
		angleX = atanf(w.m[2][1] / w.m[1][1]);
		angleZ = 0;
	}
	else
	{
		angleX = atanf(-w.m[1][2] / w.m[2][2]);
		angleZ = atanf(-w.m[0][1] / w.m[0][0]);
	}

	//float threshold = 0.001f;
	//if (abs(w.m[2][1] - 1.0) < threshold)
	//{ // R(2,1) = sin(x) = 1の時
	//	angleX = DirectX::XM_PI / 2;
	//	angleY = 0;
	//	angleZ = atan2f(w.m[1][0], w.m[0][0]);
	//}
	//else if (abs(w.m[2][1] + 1.0) < threshold) { // R(2,1) = sin(x) = -1の時
	//	angleX = -DirectX::XM_PI / 2;
	//	angleY = 0;
	//	angleZ = atan2f(w.m[1][0], w.m[0][0]);
	//}
	//else 
	//{
	//	angleX = asinf(w.m[2][1]);
	//	angleY = atan2f(-w.m[2][0], w.m[2][2]);
	//	angleZ = atan2f(-w.m[0][1], w.m[1][1]);
	//}

	rotation = DirectX::XMFLOAT4(angleX, angleY, angleZ, 1.0f);
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
	DirectX::XMStoreFloat3(&ret, rotationMatrix.r[2]);
	return ret;
}
