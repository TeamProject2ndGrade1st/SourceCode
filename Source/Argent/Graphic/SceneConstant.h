#pragma once
#include<DirectXMath.h>

namespace Argent::Graphics
{
	static constexpr int NumPointLight = 4;
	struct DirectionalLight
	{
		DirectX::XMFLOAT4 direction;
		//float pad;
		DirectX::XMFLOAT4 color;
	};

	struct PointLight
	{
		DirectX::XMFLOAT3 position;
		float range;
		DirectX::XMFLOAT4 color;
	};

	struct SceneConstant
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4 cameraPosition;
		//float pad;
		DirectionalLight directionalLight;
		/*float pad1;
		float pad2;*/
		PointLight pointLight[NumPointLight];
	};
}