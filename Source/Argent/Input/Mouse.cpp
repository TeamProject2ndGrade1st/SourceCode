#include "Mouse.h"

namespace Argent::Input
{
	Mouse::Mouse()
	{
		mouseState[Mouses::LeftButton] = State::NONE;
		mouseState[Mouses::RightButton] = State::NONE;
		mouseState[Mouses::MiddleButton] = State::NONE;
	}

	void Mouse::Update()
	{
		POINT p{};
		GetCursorPos(&p);
		//ScreenToClient(Argent::Graphics::ArGraphics::Instance()->GetWindowHandle(), &p);
		postPosition = position;
		position = DirectX::XMFLOAT2(static_cast<float>(p.x), static_cast<float>(p.y));
		moveVec = DirectX::XMFLOAT2(position.x - postPosition.x, position.y - postPosition.y);


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

	bool Mouse::IsButtonPress(Mouses m)
	{
		if (mouseState[m] == State::PRESS) return true;
		return false;
	}

	bool Mouse::IsButtonPressEnter(Mouses m)
	{
		if (mouseState[m] == State::ENTER) return true;
		return false;
	}

	bool Mouse::IsButtonRelease(Mouses m)
	{
		if (mouseState[m] == State::RELEASE) return true;
		return false;
	}
}