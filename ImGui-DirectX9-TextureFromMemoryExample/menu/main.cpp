/*


	[+] Here's a simplified guide to loading images in ImGui with DirectX 9   |   Made with love by estetik, happy coding! [+]

	Welcome to this code snippet for loading images into your ImGui menu with ImGui DirectX9! Follow these steps to set up and use the code:

	You can zoom in/out with ctrl + scroll up/down!

		Step 1: Set up your project configuration:

			• Ensure that your IDE is set to "Release -> x86" mode. (You can find this near the top of your IDE next to the 'Local Windows Debugger' button)
			• Check and adjust the project properties as follows:
			• Configuration Properties -> General -> C++ Language Standard: ISO C++20 Standard (/std:c++20).
			• Configuration Properties -> Advanced -> Character Set: Use Multi-Byte Character Set.
			• Linker -> Input -> Additional Dependencies: Add "d3d9.lib".
			• Linker -> System -> SubSystem: Set to "Windows (/SUBSYSTEM:WINDOWS)".
			
			Step 2: If you encounter any issues, feel free to contact me on Discord: estkz or e9.

		If your menu is loading up successfully, continue with the following tutorial:

			Step 1: Download and install HxD from the following link: https://mh-nexus.de/en/hxd/.
			Step 2: Open HxD and drag the picture you want to load into ImGui onto the HxD window. The hexadecimal bytes of the image will be displayed.
			Step 3: Go to "File" -> "Export" and click the "C" button. Save the file.
			Step 4: Right-click the exported file, choose "Open With," and select "Notepad" (or "Edit" if available).
			Step 5: Copy the code starting from "unsigned char rawData[bytes] = { all the bytes };" (excluding the path to the picture).
			Step 6: Create a new header file called "logo.h" (refer to the provided "logo.h" in this project if you need an example).
			Step 7: Paste the copied bytes into the newly created "logo.h" file, making sure to keep the "#pragma once" directive.
			Step 8: Rename "RawData" to a name of your choice. Remember this name as we'll use it later. For example, let's use "eclipse_logo" in this example.
			Step 9: In your "gui.cpp" file, include the logo header file using the appropriate path. For example, #include "../Logo/logo.h". (A single dot (".") refers to the current directory.
					Two consecutive dots ("..") refer to the parent directory).

			Step 10: Declare the following line of code near the top of the "gui.cpp" file:
			IDirect3DTexture9* eclipseLogo = nullptr;

			Step 11: Inside the render function, define the size of your image with the following code:
			ImVec2 image_size(655, 70);

			Step 12: Load the image bytes into a texture within the render function:
			if (single_creation) {
				D3DXCreateTextureFromFileInMemoryEx(gui::device, &eclipse_logo, sizeof(eclipse_logo), 400, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &eclipseLogo);
				single_creation = false;
			}

			Step 13: Define the following line OUTSIDE the render function to resolve any undefined error:
			bool single_creation = true;

			Step 14: Finally, render the image in the ImGui menu by adding the following code to the render function:
			ImGui::Image(eclipseLogo, image_size);






		Please note: If you encounter any issues or errors related to the includes on "d3dx9.h," make sure you have installed the DirectX SDK from https://www.microsoft.com/en-us/download/details.aspx?id=6812.

			To resolve the include errors, follow these additional steps:

				• Open the project properties.
				• Go to "C/C++ -> General -> Additional Include Directories" and click the arrow on the right.
				• Click "Edit" and add the following text: "$(DXSDK_DIR)include".
				• Go to "VC++ Directories -> Library Directories," click the arrow again.
				• Click "Edit" and add the following: "$(DXSDK_DIR)LIB\x86".
				• Click "Apply" to save the changes.
				• Once completed, the error should be resolved.

			Now you should be able to load and display images in your ImGui menu. Feel free to reach out via Discord if you need further assistance!
*/

#include "gui.h"

#include <thread>

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	// Create GUI
	gui::CreateHWindow("Software");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::isRunning)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// Destroy GUI
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}
