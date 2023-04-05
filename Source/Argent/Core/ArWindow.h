#pragma once
#include <Windows.h>


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace Argent::Window
{
	class ArWindow
	{
	public:
		ArWindow(HINSTANCE hInstance, LONG width, LONG height);
		~ArWindow() = default;


		static LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		



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

