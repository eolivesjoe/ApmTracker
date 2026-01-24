#pragma once

#include <windows.h>
#include "../tracker/ApmTracker.h"

namespace window
{
	class Window
	{
	public:
		Window(HINSTANCE hInstance, int nCmdShow, tracker::ApmTracker* tracker);
		~Window();
		int run();

	private:
		static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void initializeWindow();
		void onPaint();

		HINSTANCE m_hInstance;
		HWND m_hwnd;
		int m_nCmdShow;
		HFONT m_hFont;

		tracker::ApmTracker* m_tracker;
	};
} // namespace window
