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

		void Start();
		void Stop();
		void ResetSession();

		static int GetApm();

	private:
		static HHOOK m_keyboardHook;
		static HHOOK m_mouseHook;
		static void SetHooks(void);
		static void RemoveHooks(void);
		static LRESULT CALLBACK KeyboardProc(int nCode, WORD wParam, LONG lParam);
		static LRESULT CALLBACK MouseProc(int nCode, WORD wParam, LONG lParam);

		void Tick();
		void IncrementSecond();
		static void AddAction();
		int CalculateAPM();
		static void SetApm(int newApm);

		std::thread t;

		static std::mutex m_lock;
		static std::vector<int> m_actionsPerSecond;
		const int m_apmWindow = 60;
		static int m_currentApm;
		int m_rollingActions;
		std::atomic<bool> m_running;
	};
} // namespace tracker