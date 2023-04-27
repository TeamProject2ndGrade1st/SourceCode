// ReSharper disable All
#include "Transform.h"

#include "../GameObject/GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "../Math/MathHelper.h"

DirectX::XMFLOAT3 Transform::CalcEulerAngleFromRotationMatrix(const DirectX::XMMATRIX& mat)
{
	DirectX::XMFLOAT3 r0, r1, r2;

	DirectX::XMStoreFloat3(&r0, mat.r[0]);
	DirectX::XMStoreFloat3(&r1, mat.r[1]);
	DirectX::XMStoreFloat3(&r2, mat.r[2]);

	float sy = sqrt(r0.x * r0.x +  r1.x );


    bool singular = sy < 1e-6; // If
 
    float x, y, z;
    if (!singular)
    {
        x = atan2(r2.y , r2.z);
        y = atan2(-r2.x, sy);
        z = atan2(r1.x, r0.x);
    }
    else
    {
        x = atan2(-r1.z, r1.y);
        y = atan2(-r2.x, sy);
        z = 0;
    }
    return DirectX::XMFLOAT3(x, y, z);
}

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

void Transform::Initialize()
{
	const DirectX::XMVECTOR Axis = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMStoreFloat4(&orientation, DirectX::XMQuaternionRotationAxis(Axis, 0));
}

void Transform::Update()
{
	
}

void Transform::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
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
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&w);

	//移動値を抜き出す
	DirectX::XMFLOAT3 transration;
	DirectX::XMStoreFloat3(&transration, W.r[3]);
	//オフセットの位置はリセット
	W.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);


	//スケール行列を求めていく
	float sX, sY, sZ;
	sX = LengthV3(W.r[0]);
	sY = LengthV3(W.r[1]);
	sZ = LengthV3(W.r[2]);
	//スケール行列の逆行列を求める
	DirectX::XMMATRIX S = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixScaling(sX, sY, sZ));
	//逆行列を使って回転行列を求める
	DirectX::XMMATRIX R = S * W;

	DirectX::XMFLOAT3 rot = CalcEulerAngleFromRotationMatrix(R);

	//それぞれの値をセットしていく
	position = DirectX::XMFLOAT3(transration.x, transration.y, transration.z);
	scale = DirectX::XMFLOAT3(sX, sY, sZ);
	rotation = DirectX::XMFLOAT4(DirectX::XMConvertToDegrees(rot.x),
		DirectX::XMConvertToDegrees(rot.y),
		DirectX::XMConvertToDegrees(rot.z), 0);

}

DirectX::XMMATRIX Transform::CalcWorldMatrix()
{
	DirectX::XMMATRIX parentMatrix = DirectX::XMMatrixIdentity();
	if(GetOwner()->GetParent())
	{
		parentMatrix = GetOwner()->GetParent()->GetTransform()->CalcWorldMatrix();
	}

	const DirectX::XMFLOAT4 dRotation = rotation - postRotation;
	// Y to X to Z rotation
	const DirectX::XMVECTOR Axis = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVECTOR orientationVec =  DirectX::XMQuaternionRotationAxis(Axis, 0);

	//DirectX::XMVECTOR orientationVec = DirectX::XMLoadFloat4(&orientation);
	auto q = DirectX::XMMatrixRotationQuaternion(orientationVec);
	DirectX::XMVECTOR f = DirectX::XMVector3Normalize(q.r[2]);
	DirectX::XMVECTOR u = DirectX::XMVector3Normalize(q.r[1]);
	DirectX::XMVECTOR r = DirectX::XMVector3Normalize(q.r[0]);

	const DirectX::XMMATRIX C = { DirectX::XMLoadFloat4x4(&CoordinateSystemTransforms[coordinateSystem]) *
		DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
	const DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) };

	orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(u, DirectX::XMConvertToRadians(rotation.y)));
	orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(r, DirectX::XMConvertToRadians(rotation.x)));
	orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(f, DirectX::XMConvertToRadians(rotation.z)));
	//orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(u, DirectX::XMConvertToRadians(dRotation.y)));
	//orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(r, DirectX::XMConvertToRadians(dRotation.x)));
	//orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(f, DirectX::XMConvertToRadians(dRotation.z)));

	const DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(orientationVec);
	const DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x,position.y, position.z) };
	//const DirectX::XMMATRIX Dw = DirectX::XMLoadFloat4x4(&defaultWorld);


	postRotation = rotation;
	DirectX::XMStoreFloat4(&orientation, orientationVec);
	//return W * parentMatrix;
	if(GetOwner()->GetParent())
	{
		DirectX::XMMATRIX W = S * R * T;
		return W * parentMatrix;
	}
	else
	{
		DirectX::XMMATRIX W = C * S * R * T;
		return W * parentMatrix;
	}
}

DirectX::XMMATRIX Transform::CalcLocalMatrix()
{
	const DirectX::XMFLOAT4 dRotation = rotation - postRotation;
	// Y to X to Z rotation
	DirectX::XMVECTOR orientationVec = DirectX::XMLoadFloat4(&orientation);
	auto q = DirectX::XMMatrixRotationQuaternion(orientationVec);
	DirectX::XMVECTOR f = DirectX::XMVector3Normalize(q.r[2]);
	DirectX::XMVECTOR u = DirectX::XMVector3Normalize(q.r[1]);
	DirectX::XMVECTOR r = DirectX::XMVector3Normalize(q.r[0]);

	const DirectX::XMMATRIX C = { DirectX::XMLoadFloat4x4(&CoordinateSystemTransforms[coordinateSystem]) *
		DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
	const DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) };

	orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(u, DirectX::XMConvertToRadians(dRotation.y)));
	orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(r, DirectX::XMConvertToRadians(dRotation.x)));
	orientationVec = DirectX::XMQuaternionMultiply(orientationVec, DirectX::XMQuaternionRotationAxis(f, DirectX::XMConvertToRadians(dRotation.z)));

	const DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(orientationVec);
	const DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x,position.y, position.z) };
	//const DirectX::XMMATRIX Dw = DirectX::XMLoadFloat4x4(&defaultWorld);
	postRotation = rotation;
	DirectX::XMStoreFloat4(&orientation, orientationVec);
	return  (C * S * R * T);

}

//Transform Transform::AdjustParentTransform() const
//{
//	Transform tmp = *this;
//	//tmp += *this;
//	if(GetOwner())
//	{
//		if(GetOwner()->GetParent())
//		{
//			const Transform t = GetOwner()->GetParent()->GetTransform()->AdjustParentTransform();
//			tmp += t;
//			tmp.scale = tmp.scale * t.scale;
//			tmp.scaleFactor *= t.GetScaleFactor();
//			tmp.coordinateSystem = t.coordinateSystem;
//		}
//	}
//	return tmp;
//}

void Transform::SetTransform(const Transform& t)
{
	*this = t; 
}

DirectX::XMFLOAT3 Transform::CalcForward()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z));
	DirectX::XMFLOAT3 ret{};
	DirectX::XMStoreFloat3(&ret, DirectX::XMVector3Normalize(rotationMatrix.r[2]));
	return ret;
}

DirectX::XMFLOAT3 Transform::CalcUp()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z));
	DirectX::XMFLOAT3 ret{};
	DirectX::XMStoreFloat3(&ret, DirectX::XMVector3Normalize(rotationMatrix.r[1]));
	return ret;
}
DirectX::XMFLOAT3 Transform::CalcRight()
{

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z));
	DirectX::XMFLOAT3 ret{};
	DirectX::XMStoreFloat3(&ret, DirectX::XMVector3Normalize(rotationMatrix.r[0]));
	return ret;
}
