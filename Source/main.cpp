#include "app/App.h"
#include "logger/Logger.h"

int main(int argc, char* argv[])
{
    logger::init();

    app::App app;
    logger::info("starting tracker...");
    app.run();

    return 0;
}
