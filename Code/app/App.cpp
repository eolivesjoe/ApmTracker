#include <windows.h>

#include "app.h"
#include "../window/window.h"
#include "../tracker/ApmTracker.h"

namespace app
{
	int App::run()
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		int nCmdShow = SW_SHOWDEFAULT;

		tracker::ApmTracker tracker;
		tracker.start();

		window::Window window(hInstance, nCmdShow, &tracker);
		window.run();

		tracker.stop();
		return 0;
	}
}