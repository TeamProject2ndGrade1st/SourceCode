#pragma once
#include <DirectXMath.h>
#include "../../../imgui/imgui.h"

namespace Argent
{
	struct Color
	{
		Color(const DirectX::XMFLOAT4& color = DirectX::XMFLOAT4(1, 1, 1, 1)):
			color(color)
		{}

		void Reset()
		{
			color.x = color.y = color.z = color.w = 1.0f;
		}

		void DrawDebug()
		{
			ImGui::ColorEdit4("Color", &color.x,
				ImGuiColorEditFlags_PickerHueWheel | 
				ImGuiColorEditFlags_AlphaBar);
		}

		DirectX::XMFLOAT4 GetColor() const { return color;  }
		DirectX::XMFLOAT4 color;
	};
}
