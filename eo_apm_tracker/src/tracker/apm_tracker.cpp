#include "apm_tracker.h"
#include "../constants.h"

namespace tracker
{
	HHOOK ApmTracker::m_keyboardHook = NULL;
	HHOOK ApmTracker::m_mouseHook = NULL;
	std::mutex ApmTracker::m_lock;
	std::vector<int> ApmTracker::m_actionsPerSecond;
	int ApmTracker::m_currentApm = 0;

	ApmTracker::ApmTracker()
	{
		m_actionsPerSecond.push_back(0);
		this->m_rollingActions = 0;
		this->m_running = true;
	}

	ApmTracker::~ApmTracker()
	{

	}

	void ApmTracker::Start()
	{
		SetHooks();
		t = std::thread(&ApmTracker::Tick, this);
	}

	void ApmTracker::Stop()
	{
		m_running = false;
		RemoveHooks();
		if (t.joinable())
		{
			t.join();
		}
	}

	void ApmTracker::Tick()
	{
		while (m_running)
		{
			IncrementSecond();
			Sleep(1000);
		}
	}

	void ApmTracker::IncrementSecond()
	{
		int calculated_apm = CalculateAPM();

		const std::lock_guard<std::mutex> lock(m_lock);
		SetApm(calculated_apm);
		m_actionsPerSecond.push_back(0);

		if (m_actionsPerSecond.size() > MAX_HISTORY)
		{
			m_rollingActions -= m_actionsPerSecond.front();
			m_actionsPerSecond.erase(m_actionsPerSecond.begin());
		}
	}

	void ApmTracker::AddAction()
	{
		const std::lock_guard<std::mutex> lock(m_lock);
		if (!m_actionsPerSecond.empty())
		{
			++m_actionsPerSecond[m_actionsPerSecond.size() - 1];
		}
	}

	int ApmTracker::CalculateAPM()
	{
		int current_second = m_actionsPerSecond.size() - 1;

		if (current_second < 1)
			return 0;

		m_rollingActions += m_actionsPerSecond[current_second];
		if (m_actionsPerSecond.size() > m_apmWindow)
		{
			m_rollingActions -= m_actionsPerSecond[current_second - m_apmWindow];
			return m_rollingActions;
		}
		float apm = static_cast<float>(m_apmWindow) / static_cast<float>(current_second);
		return static_cast<int>(apm * m_rollingActions);
	}

	void ApmTracker::SetApm(int new_apm)
	{
		m_currentApm = new_apm;
	}

	int ApmTracker::GetApm()
	{
		return m_currentApm;
	}

	void ApmTracker::SetHooks(void)
	{
		m_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, 0, 0);
		m_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, 0, 0);
	}

	void ApmTracker::RemoveHooks(void)
	{
		UnhookWindowsHookEx(m_keyboardHook);
		UnhookWindowsHookEx(m_mouseHook);
	}

	LRESULT CALLBACK ApmTracker::KeyboardProc(int nCode, WORD wParam, LONG lParam)
	{
		if (nCode >= 0 && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			AddAction();

		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	LRESULT CALLBACK ApmTracker::MouseProc(int nCode, WORD wParam, LONG lParam)
	{
		if (nCode >= 0 && (
			wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP ||
			wParam == WM_MBUTTONUP || wParam == WM_XBUTTONUP))
		{
			AddAction();
		}

		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	void ApmTracker::ResetSession()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_actionsPerSecond.clear();
		m_actionsPerSecond.push_back(0);
		m_rollingActions = 0;
		m_currentApm = 0;
	}
}