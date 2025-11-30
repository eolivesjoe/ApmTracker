#include <windows.h>

#include "app.h"
#include "../window/window.h"
#include "../tracker/ApmTracker.h"

namespace app
{
	int App::Run()
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		int nCmdShow = SW_SHOWDEFAULT;

		tracker::ApmTracker tracker;
		tracker.Start();

		window::Window window(hInstance, nCmdShow, &tracker);
		window.Run();

		tracker.Stop();
		return 0;
	}
}