#include <string>

#include "Window.h"
#include "../Constants.h"

namespace window
{
	Window::Window(HINSTANCE hInstance, int nCmdShow, tracker::ApmTracker* tracker)
		: m_hInstance(hInstance), m_nCmdShow(nCmdShow), m_hwnd(nullptr), m_hFont(nullptr), m_tracker(tracker)
	{
		InitializeWindow();

		m_hFont = CreateFontA(
			48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Arial");
	}

	Window::~Window()
	{
		UnregisterClass("APM Tracker", m_hInstance);

		if (m_hFont)
		{
			DeleteObject(m_hFont);
			m_hFont = nullptr;
		}
	}

	void Window::InitializeWindow()
	{
		WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = Window::WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = "APM Tracker";
		wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		RegisterClassEx(&wc);

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int x = screenWidth - WIDTH - 10;
		int y = screenHeight - HEIGHT - (screenHeight / 3);

		m_hwnd = CreateWindowEx(
			WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
			wc.lpszClassName,
			nullptr,
			WS_POPUP,
			x, y,
			100, 100,
			nullptr,
			nullptr,
			m_hInstance,
			this
		);

		SetTimer(m_hwnd, TIMER_ID, TIMER_INTERVAL_MS, NULL);
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(m_hwnd, SW_SHOWNOACTIVATE);
		UpdateWindow(m_hwnd);

		SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
	}

	int Window::Run()
	{
		MSG msg = {};
		while (GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return static_cast<int>(msg.wParam);
	}

	LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		switch (msg)
		{
		case WM_PAINT:
		{
			if (window) 
			{
				window->OnPaint();
			}
			return 0;
		}

		case WM_TIMER:
		{
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}

		case WM_DESTROY:
		{
			KillTimer(hwnd, TIMER_ID);
			PostQuitMessage(0);
			return 0;
		}

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	void Window::OnPaint()
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		RECT rect;
		GetClientRect(m_hwnd, &rect);

		int apm = 0;
		if (m_tracker)
			apm = m_tracker->GetApm();

		std::string text = std::to_string(apm);
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkMode(hdc, TRANSPARENT);

		HFONT hOldFont = (HFONT)SelectObject(hdc, m_hFont);
		DrawTextA(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		SelectObject(hdc, hOldFont);

		EndPaint(m_hwnd, &ps);
	}
}