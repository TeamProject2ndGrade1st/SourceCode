#include "Mouse.h"
#include "../Graphic/Graphics.h"

#include "Keyboard.h"


namespace Argent::Input
{
	Mouse::Mouse()
	{
		mouseState[Button::LeftButton] = State::NONE;
		mouseState[Button::RightButton] = State::NONE;
		mouseState[Button::MiddleButton] = State::NONE;
	}

	void Mouse::Update()
	{
		if(Argent::Input::Keyboard::Instance().GetKeyUp(Keyboard::F1))
		{
			resetPositionToCenter = !resetPositionToCenter;
		}
		POINT p{};
		GetCursorPos(&p);
		//ScreenToClient(Argent::Graphics::Graphics::Instance()->hWnd, &p);
		if(resetPositionToCenter)
		{
			postPosition =DirectX::XMFLOAT2(width / 2, height / 2);
			ShowCursor(false);
		}
		else
		{
			ShowCursor(true);
			postPosition = position;
		}
		position = DirectX::XMFLOAT2(static_cast<float>(p.x), static_cast<float>(p.y));
		moveVec = DirectX::XMFLOAT2(position.x - postPosition.x, position.y - postPosition.y);

		if(resetPositionToCenter)
		{
			SetCursorPos(width / 2, height / 2);
		}

		for (auto& m : mouseState)
		{
			const SHORT State = GetKeyState(static_cast<int>(m.first));
			if (State & 0x8000)
			{
				switch (m.second)
				{
				case State::NONE:
					m.second = State::ENTER;
					break;

				case State::ENTER:
				case State::PRESS:
					m.second = State::PRESS;
					break;

				case State::RELEASE:
					m.second = State::NONE;
					break;
				}
			}
			else
			{
				switch (m.second)
				{
				case State::NONE:
				case State::RELEASE:
					m.second = State::NONE;
					break;

				case State::PRESS:
				case State::ENTER:
					m.second = State::RELEASE;
					break;
				}
			}
		}


		if (!isWheelRotate) wheelState = WheelStates::None;
		else if (rowWheelRotateValue > 0) wheelState = WheelStates::Plus;
		else if (rowWheelRotateValue < 0) wheelState = WheelStates::Minus;
		else wheelState = WheelStates::None;
	}

	bool Mouse::GetButton(Button m)
	{
		if (mouseState[m] == State::PRESS) return true;
		return false;
	}

	bool Mouse::GetButtonDown(Button m)
	{
		if (mouseState[m] == State::ENTER) return true;
		return false;
	}

	bool Mouse::GetButtonUp(Button m)
	{
		if (mouseState[m] == State::RELEASE) return true;
		return false;
	}
}