#include "gui.h"

#include <thread>

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE prev, PWSTR args, int show)
{
    gui::CreateHWindow("Software");

    if (gui::CreateDevice())
    {
        gui::CreateImGui();

        while (gui::isRunning)
        {
            gui::BeginRender();
            gui::Render();
            gui::EndRender();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        gui::DestroyImGui();
        gui::DestroyDevice();
    }

    gui::DestroyHWindow();
    return EXIT_SUCCESS;
}