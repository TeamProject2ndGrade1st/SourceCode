#pragma once
#include <DirectXMath.h>
#include <unordered_map>
#include "Keyboard.h"

namespace Argent::Input
{
	class Mouse
	{
	public:
		enum class Button
		{
			LeftButton = 0x01,
			RightButton = 0x02,
			MiddleButton = 0x04,
		};

		enum class WheelStates
		{
			None,
			Plus,
			Minus
		};

	private:
		Mouse();
		~Mouse() = default;

	public:
		static Mouse& Instance()
		{
			static Mouse instance;
			return instance;
		}


		void Update();
		bool GetButton(Button m);
		bool GetButtonDown(Button m);
		bool GetButtonUp(Button m);
		bool IsMouseWheelRotate() const { return isWheelRotate; }

		void SetIsWheelRotateOn() { isWheelRotate = true; }
		void SetIsWheelRotateOff() { isWheelRotate = false; }
		void SetRowWheelRotateValue(float v) { rowWheelRotateValue = v; }

		DirectX::XMFLOAT2 GetPosition() const { return position; }
		DirectX::XMFLOAT2 GetMoveVec() const { return moveVec; }
		DirectX::XMFLOAT2 GetPostPosition() const { return postPosition; }
		WheelStates GetWheelState() const { return wheelState; }

		float GetRowWheelRotateValue() const { return rowWheelRotateValue; }

		std::unordered_map<Button, State> mouseState;
	private:
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 moveVec;
		DirectX::XMFLOAT2 postPosition;

		bool isWheelRotate;
		float rowWheelRotateValue;
		WheelStates wheelState;
	};
}