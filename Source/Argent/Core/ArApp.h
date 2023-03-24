#pragma once
#include <memory>

#include "../Graphic/ArGraphics.h"
#include "ArWindow.h"
#include "ArTimer.h"
#include "../Resource/ArEffectManager.h"

namespace Argent
{
	namespace App
	{
		/**
		 * \brief �A�v���P�[�V�����N���X�@������Execute���Ăяo�������œ���
		 */
		class ArApp final
		{
		public:
			ArApp(HINSTANCE hInstance, LONG width, LONG height, const char* appName, bool isFullScreen);
			~ArApp() = default;
			ArApp(const ArApp&) = delete;
			ArApp(const ArApp&&) = delete;
			ArApp operator=(const ArApp&) = delete;
			ArApp operator=(const ArApp&&) = delete;

			/**
			 * \brief�@���s�֐�
			 * \return �I���R�[�h
			 */
			int Execute() const;

		private:

			int Terminate() const;
			void Initialize() const;

		private:
			std::unique_ptr<Argent::Graphics::ArGraphics> arGfx{};
			std::unique_ptr<Argent::Window::ArWindow> arWindow{};
			std::unique_ptr<Argent::Resource::Effect::ArEffectManager> effectManager{};
		};
	}

	inline bool MainLoop(HWND hWnd)
	{
		MSG msg{};

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Argent::Timer::ArTimer::Instance().Tick();
		Argent::Timer::ArTimer::Instance().CalcFrameTime(hWnd);

		if(WM_QUIT == msg.message) return false;

		return true;
	}
}
