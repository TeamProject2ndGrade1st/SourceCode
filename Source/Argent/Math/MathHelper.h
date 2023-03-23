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
	ret.x = f1.x - f2.x;

}