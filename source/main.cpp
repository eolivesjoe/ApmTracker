#include "app/App.h"
#include "logger/Logger.h"

int main(int argc, char* argv[])
{
    logger::Init();

    app::App app;
    logger::Info("starting tracker...");
    app.Run();

    return 0;
}
