#pragma once
#include <memory>

#include "../Graphic/Graphics.h"
#include "ArWindow.h"
#include "Timer.h"
#include "../Resource/EffectManager.h"

namespace Argent
{
	namespace App
	{
		/**
		 * \brief アプリケーションクラス　こいつのExecuteを呼び出すだけで動く
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
			 * \brief　実行関数
			 * \return 終了コード
			 */
			int Execute() const;

			/**
			 * \brief アプリケーション終了用の関数
			 */
			static void Quit();
		private:
			int Terminate() const;
			void Initialize() const;

		private:
			std::unique_ptr<Argent::Graphics::Graphics> arGfx{};
			std::unique_ptr<Argent::Window::ArWindow> window{};
			std::unique_ptr<Argent::Resource::Effect::EffectManager> effectManager{};

			static bool quit;
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
