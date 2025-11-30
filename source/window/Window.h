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
		int Run();

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void InitializeWindow();
		void OnPaint();

		HINSTANCE m_hInstance;
		HWND m_hwnd;
		int m_nCmdShow;
		HFONT m_hFont;

		tracker::ApmTracker* m_tracker;
	};
} // namespace window
