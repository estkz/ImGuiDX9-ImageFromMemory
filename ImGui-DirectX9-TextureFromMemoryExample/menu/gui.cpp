// Ignore the following 2 includes!
#include <windows.h>
#include <shellapi.h>
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "gui.h" // NOTE: Include your logo file
#include "../Logo/logo.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

// NOTE: If you encounter errors with the 4 includes below, please read the alert below!
#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// !!!ALERT!!!: 
// IF YOU HAVE ISSUES/ERRORS WITH THE #includes on d3dx9.h, MAKE SURE YOU HAVE INSTALLED THE DirectX SDK! 

// DOWNLOAD HERE: https://www.microsoft.com/en-us/download/details.aspx?id=6812 


// FOR MORE INFO ON THIS MATTER, HEAD TO main.cpp: Line 57

/* 

	NOTE: Please note that ALL the comments throughout the code marked with a small note are crucial for image loading. 
	Please be aware that functions without the "NOTE:" comment serve purposes other than image loading. These functions are utilized for various tasks unrelated to image loading.
	To simplify the process, ensure you have the readme file from the GitHub repository at hand. 
		 
*/


// NOTE: Declares a pointer variable named "eclipseLogo" of type "IDirect3DTexture9*" and initializes it to nullptr.
IDirect3DTexture9* eclipseLogo = nullptr;




extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU)
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter);
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "software";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"software",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}


// NOTE: Put this above your Render function:
bool single_creation = true;

// NOTE: THIS IS YOUR RENDER FUNCTION!
void gui::Render() noexcept
{
	// You can ignore the following lines of code, please continue on line 299.
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin("Load Image From Bytes", &isRunning, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	if (ImGui::Button("github.com/estkz"))
	{
		ShellExecute(0, 0, "https://github.com/estkz/ImGuiDX9-ImageFromMemory", 0, 0, SW_SHOW); // Opens my github in your browser.
	}

	//
	// NOTE: BEGIN OF IMAGE LOADING
	//
	

	// NOTE: Size of your image : Width x Height
	ImVec2 image_size(200, 75);

	if (single_creation) // This if-statement makes sure the Texture only gets created once. If you do not do this your program will lag or even crash!!!
	{
		D3DXCreateTextureFromFileInMemoryEx(gui::device, &eclipse_logo, sizeof(eclipse_logo), 400, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &eclipseLogo);
		single_creation = false;
	}

	// PARAMETERS OF D3DXCreateTextureFromFileInMemoryEx EXPLAINED: 
	/*
	
		• gui::device: This parameter is the IDirect3DDevice9* pointer representing the Direct3D device. It is used to create the texture.
		• &eclipse_logo: This is the address of the pointer variable that will hold the created texture.
		• sizeof(eclipse_logo): This parameter specifies the size, in bytes, of the memory block pointed to by &eclipse_logo.
		• 400: The width of the texture in pixels.
		• 50: The height of the texture in pixels.
		• D3DX_DEFAULT: This parameter specifies the number of mip levels. Using D3DX_DEFAULT creates a full set of mip levels.
		• 0: The usage flags for the texture. In this case, it indicates no special usage.
		• D3DFMT_UNKNOWN: The pixel format of the texture. D3DFMT_UNKNOWN allows Direct3D to choose the format based on the source image.
		• D3DPOOL_DEFAULT: This parameter specifies the memory pool where the texture is stored. D3DPOOL_DEFAULT indicates that the texture is stored in the default pool, allowing the driver to choose the appropriate memory location.
		• D3DX_DEFAULT: The filter type used when generating mipmaps. D3DX_DEFAULT lets the function choose an appropriate filter.
		• D3DX_DEFAULT: The mip filter type used when downsampling mipmaps. Again, D3DX_DEFAULT allows the function to choose an appropriate filter.
		• 0: The color key value used for transparency. In this case, no color key is used.
		• NULL: Pointer to a D3DXIMAGE_INFO structure that receives information about the loaded image. Since NULL is passed, no information is returned.
		• NULL: Pointer to a palette for palettized texture formats. Since NULL is passed, no palette is used.
		• &eclipseLogo: This is the address of the pointer that will receive the created texture object.
	*/


	// Optional for if you want to take the available space when using childforms for example:
	// ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (image_size.x / 2));

	// NOTE: Render the image by specifying the pointer variable named "eclipseLogo" (near the top of this .cpp file) and the image_size you've provided.
	ImGui::Image(eclipseLogo, image_size);

	ImGui::End();
}
