#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include <vector>

namespace tracker
{
	class ApmTracker
	{
	public:
		ApmTracker();
		~ApmTracker();

		void start();
		void stop();
		void resetSession();

		static int getApm();

	private:
		static HHOOK m_keyboardHook;
		static HHOOK m_mouseHook;
		static void setHooks(void);
		static void removeHooks(void);
		static LRESULT CALLBACK keyboardProc(int nCode, WORD wParam, LONG lParam);
		static LRESULT CALLBACK mouseProc(int nCode, WORD wParam, LONG lParam);

		void tick();
		void incrementSecond();
		static void addAction();
		int calculateAPM();
		static void setApm(int newApm);

		std::thread t;

		static std::mutex m_lock;
		static std::vector<int> m_actionsPerSecond;
		const int m_apmWindow = 60;
		static int m_currentApm;
		int m_rollingActions;
		std::atomic<bool> m_running;
	};
} // namespace tracker