#pragma once
#include <Windows.h>
#include "../Input/Keyboard.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace Argent::Window
{
	class ArWindow
	{
	public:
		ArWindow(HINSTANCE hInstance, LONG width, LONG height);
		~ArWindow() = default;


		static LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			//todo �ǂ��ŌĂԂׂ��H
			Input::Mouse::Instance().SetIsWheelRotateOff();
			Input::Mouse::Instance().SetRowWheelRotateValue(0);

			switch (msg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			case WM_MOUSEWHEEL:
				Input::Mouse::Instance().SetRowWheelRotateValue(GET_WHEEL_DELTA_WPARAM(wParam));
				Input::Mouse::Instance().SetIsWheelRotateOn();

				break;

			default:
				break;
			}
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}



		HWND GetHandle() const { return hWnd; }
	private:
		HWND hWnd{};
	};


	inline LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		ArWindow* p{reinterpret_cast<ArWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
		return p ? p->HandleMessage(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
	}


}

