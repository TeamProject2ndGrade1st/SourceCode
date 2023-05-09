#pragma once
#include<DirectXMath.h>

namespace Argent::Graphics
{
	static constexpr int NumPointLight = 4;
	struct DirectionalLight
	{
		DirectX::XMFLOAT3 direction;
		//float pad;
		DirectX::XMFLOAT4 color;
	};

	struct PointLight
	{
		float pad;
		float pad1;
		DirectX::XMFLOAT3 position;
		float range;
		DirectX::XMFLOAT4 color;
	};

	struct SceneConstant
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT3 cameraPosition;
		DirectionalLight directionalLight;
		PointLight pointLight;
	};
}