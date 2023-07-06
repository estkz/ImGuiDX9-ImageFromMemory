#pragma once
#include <d3d9.h>

// Here you can find the gui namespace, feel free to take a look at all the functions!

/*
	Short breakdown of the functions:
	
		#pragma once: This directive ensures that the header file is included only once during compilation.
		#include <d3d9.h>: This line includes the DirectX 9 header file, which is needed for working with DirectX 9 graphics.
		namespace gui: The code is enclosed within the "gui" namespace, which is used to group related variables and functions together.

			Various variable declarations:

				• WIDTH and HEIGHT: Constants representing the window size.
				• isRunning: A boolean flag indicating whether the program is running.
				• window: A handle to the WinAPI window.
				• windowClass: A structure representing the WinAPI window class.
				• position: A structure representing the coordinates of the window position.
				• d3d: A pointer to the IDirect3D9 interface, used for managing the DirectX 9 rendering subsystem.
				• device: A pointer to the IDirect3DDevice9 interface, used for rendering graphics.
				• presentParameters: A structure specifying the presentation parameters for creating the device.
		
		[+] Function declarations:

				• CreateHWindow: Creates the WinAPI window.
				• DestroyHWindow: Destroys the WinAPI window.
				• CreateDevice: Creates the DirectX 9 device.
				• ResetDevice: Resets the DirectX 9 device.
				• DestroyDevice: Destroys the DirectX 9 device.
				• CreateImGui: Initializes ImGui.
				• DestroyImGui: Shuts down ImGui.
				• BeginRender: Begins the ImGui rendering process.
				• EndRender: Ends the ImGui rendering process.
				• Render: Renders the ImGui interface.

*/

namespace gui
{
	// Variables
	constexpr int WIDTH = 500;
	constexpr int HEIGHT = 300;

	inline bool isRunning = true;

	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };

	inline POINTS position = { };

	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = { };

	// Functions -> Expand in gui.cpp
	void CreateHWindow(const char* windowName) noexcept;
	void DestroyHWindow() noexcept;

	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
}
