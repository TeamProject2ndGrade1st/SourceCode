#pragma once
#include <DirectXMath.h>

inline DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& f1, const DirectX::XMFLOAT3& f2)
{
	DirectX::XMFLOAT3 ret{};
	ret.x = f1.x + f2.x;
	ret.y = f1.y + f2.y;
	ret.z = f1.z + f2.z;
	return ret;
} 

inline DirectX::XMFLOAT3  operator-(const DirectX::XMFLOAT3& f1, const DirectX::XMFLOAT3& f2)
{
	DirectX::XMFLOAT3 ret{};
	ret.x = f1.x - f2.x;
	ret.y = f1.y - f2.y;
	ret.z = f1.z - f2.z;
	return ret;
}

inline DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& f1, const DirectX::XMFLOAT3& f2)
{
	DirectX::XMFLOAT3 ret{};
	ret.x = f1.x * f2.x;
	ret.y = f1.y * f2.y;
	ret.z = f1.z * f2.z;
	return ret;
}


inline DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& f1, float f)
{
	DirectX::XMFLOAT3 ret{};
	ret.x = f1.x * f;
	ret.y = f1.y * f;
	ret.z = f1.z * f;

	return ret;
}

inline DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& f1, const DirectX::XMFLOAT4& f2)
{
	DirectX::XMFLOAT4 ret{};
	ret.x = f1.x + f2.x;
	ret.y = f1.y + f2.y;
	ret.z = f1.z + f2.z;
	ret.w = f1.w + f2.w;
	return ret;
}

inline DirectX::XMFLOAT3 Normalize(const DirectX::XMFLOAT3& f)
{
	DirectX::XMFLOAT3 r;
	DirectX::XMStoreFloat3(&r, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&f)));
	return r;
}

inline DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& f1, const DirectX::XMFLOAT4& f2)
{
	DirectX::XMFLOAT4 ret{};
	ret.x = f1.x - f2.x;
	ret.y = f1.y - f2.y;
	ret.z = f1.z - f2.z;
	ret.w = f1.w - f2.w;
	return ret;
}

inline DirectX::XMFLOAT4 Max(const DirectX::XMFLOAT4& f1, const DirectX::XMFLOAT4& f2)
{
	DirectX::XMFLOAT4 ret{};
	ret.x = f1.x > f2.x ? f1.x : f2.x;
	ret.y = f1.y > f2.y ? f1.y : f2.y;
	ret.z = f1.z > f2.z ? f1.z : f2.z;
	ret.w = f1.w > f2.w ? f1.w : f2.w;
	return ret;
}

inline DirectX::XMFLOAT4 Min(const DirectX::XMFLOAT4& f1, const DirectX::XMFLOAT4& f2)
{
	DirectX::XMFLOAT4 ret{};
	ret.x = f1.x < f2.x ? f1.x : f2.x;
	ret.y = f1.y < f2.y ? f1.y : f2.y;
	ret.z = f1.z < f2.z ? f1.z : f2.z;
	ret.w = f1.w < f2.w ? f1.w : f2.w;
	return ret;
}

inline float Length(const DirectX::XMFLOAT3& f1, const DirectX::XMFLOAT3&f2)
{
	float ret{};
	DirectX::XMStoreFloat(
		&ret, 
		DirectX::XMVector3Length(
			DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&f1), 
				DirectX::XMLoadFloat3(&f2))));
	return ret;
}

inline float Length(const DirectX::XMFLOAT3& f)
{
	float ret;
	DirectX::XMStoreFloat(&ret, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&f)));
	return ret;
}

inline float LengthV3(const DirectX::XMVECTOR& v)
{
	float ret;
	DirectX::XMStoreFloat(&ret, DirectX::XMVector3Length(v));
	return ret; 
}