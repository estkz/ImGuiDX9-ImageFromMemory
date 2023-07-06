# 💻 ImGuiDX9-ImageFromMemory
This code simplifies the process of loading an image from bytes into a texture and displaying your logo in ImGui. It provides a quick and efficient way to accomplish this task.

## 🌒 About
A straightforward tutorial on how to load an image into your ImGui menu by utilizing the image's memory.

## 🌌 Setup
- Clone the repo
- Follow the guide in main.cpp
- Understand the code.
- Implement it in your menu if needed.
- Build in Release | x86
- Profit

## 💎 Contact
You can contact me on Discord via the following names: estkz or e9r

## 📃 Guide
So, you want to learn how to load your image in imgui DirectX9. You're at the right place!

Firstly I will give you all the code that is needed (excluding all the other functions):
```c++

#include "gui.h" // This is the array of bytes from your image.
#include "../Logo/logo.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

IDirect3DTexture9* eclipseLogo = nullptr;

bool single_creation = true;

// The following code must be included in your ImGui Render function:
ImVec2 image_size(200, 75);

if (single_creation) // This if-statement makes sure the Texture only gets created once. If you do not do this your program will lag or even crash!!!
	{
		D3DXCreateTextureFromFileInMemoryEx(gui::device, &eclipse_logo, sizeof(eclipse_logo), 400, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &eclipseLogo);
		single_creation = false;
	}

ImGui::Image(eclipseLogo, image_size);
```
