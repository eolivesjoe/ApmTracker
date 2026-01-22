#include "ApmTracker.h"
#include "../Constants.h"

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

	void ApmTracker::start()
	{
		setHooks();
		t = std::thread(&ApmTracker::tick, this);
	}

	void ApmTracker::stop()
	{
		m_running = false;
		removeHooks();
		if (t.joinable())
		{
			t.join();
		}
	}

	void ApmTracker::tick()
	{
		while (m_running)
		{
			incrementSecond();
			Sleep(1000);
		}
	}

	void ApmTracker::incrementSecond()
	{
		int calculatedApm = calculateAPM();

		const std::lock_guard<std::mutex> lock(m_lock);
		setApm(calculatedApm);
		m_actionsPerSecond.push_back(0);

		if (m_actionsPerSecond.size() > MAX_HISTORY)
		{
			m_rollingActions -= m_actionsPerSecond.front();
			m_actionsPerSecond.erase(m_actionsPerSecond.begin());
		}
	}

	void ApmTracker::addAction()
	{
		const std::lock_guard<std::mutex> lock(m_lock);
		if (!m_actionsPerSecond.empty())
		{
			++m_actionsPerSecond[m_actionsPerSecond.size() - 1];
		}
	}

	int ApmTracker::calculateAPM()
	{
		int currentSecond = m_actionsPerSecond.size() - 1;

		if (currentSecond < 1)
			return 0;

		m_rollingActions += m_actionsPerSecond[currentSecond];
		if (m_actionsPerSecond.size() > m_apmWindow)
		{
			m_rollingActions -= m_actionsPerSecond[currentSecond - m_apmWindow];
			return m_rollingActions;
		}
		float apm = static_cast<float>(m_apmWindow) / static_cast<float>(currentSecond);
		return static_cast<int>(apm * m_rollingActions);
	}

	void ApmTracker::setApm(int newApm)
	{
		m_currentApm = newApm;
	}

	int ApmTracker::getApm()
	{
		return m_currentApm;
	}

	void ApmTracker::setHooks(void)
	{
		m_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)keyboardProc, 0, 0);
		m_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)mouseProc, 0, 0);
	}

	void ApmTracker::removeHooks(void)
	{
		UnhookWindowsHookEx(m_keyboardHook);
		UnhookWindowsHookEx(m_mouseHook);
	}

	LRESULT CALLBACK ApmTracker::keyboardProc(int nCode, WORD wParam, LONG lParam)
	{
		if (nCode >= 0 && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			addAction();

		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	LRESULT CALLBACK ApmTracker::mouseProc(int nCode, WORD wParam, LONG lParam)
	{
		if (nCode >= 0 && (
			wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP ||
			wParam == WM_MBUTTONUP || wParam == WM_XBUTTONUP))
		{
			addAction();
		}

		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	void ApmTracker::resetSession()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_actionsPerSecond.clear();
		m_actionsPerSecond.push_back(0);
		m_rollingActions = 0;
		m_currentApm = 0;
	}
}