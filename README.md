# 💻 ImGuiDX9-ImageFromMemory
This code simplifies the process of loading an image from bytes into a texture and displaying your logo in ImGui. It provides a quick and efficient way to accomplish this task.

## 🌒 About
A straightforward tutorial on how to load an image into your ImGui menu by utilizing the image's memory.

## 🌌 Setup
[+] Clone the repo
[+] Follow the guide in main.cpp
[+] Understand the code.
[+] Implement it in your menu if needed.
[+] Build in Release | x86
[+] Profit

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

Now, let's take a look at all the lines of code that we'll be implementing into our menu. But before that, let's open main.cpp!

Step 1: Before proceeding, make sure you are running this project in Release -> X86 mode. Additionally, ensure that you have the correct project configuration by considering the following important aspects:

	• Ensure that your IDE is set to "Release -> x86" mode. (You can find this near the top of your IDE next to the 'Local Windows Debugger' button)
	• Check and adjust the project properties as follows:
	• Configuration Properties -> General -> C++ Language Standard: ISO C++20 Standard (/std:c++20).
	• Configuration Properties -> Advanced -> Character Set: Use Multi-Byte Character Set.
	• Linker -> Input -> Additional Dependencies: Add "d3d9.lib".
	• Linker -> System -> SubSystem: Set to "Windows (/SUBSYSTEM:WINDOWS)".

	• Step 2. Download and install HxD from the following link: https://mh-nexus.de/en/hxd/. 
	• Step 3. Open HxD and drag the picture you want to load into ImGui onto the HxD window. The hexadecimal bytes of the image will be displayed. 
	• Step 4. Step 3: Go to "File" -> "Export" and click the "C" button. Save the file. 
	• Step 5: Right-click the exported file, choose "Open With," and select "Notepad" (or "Edit" if available). 
	• Step 6: Copy the code starting from "unsigned char rawData[bytes] = { all the bytes };" (excluding the path to the picture). 
	• Step 7: Create a new header file called "logo.h" (refer to the provided "logo.h" in this project if you need an example). 
	• Step 8: Paste the copied bytes into the newly created "logo.h" file, making sure to keep the "#pragma once" directive. 
	• Step 9: Rename "RawData" to a name of your choice. Remember this name as we'll use it later. For example, let's use "eclipse_logo" in this example. 
	• Step 10: In your "gui.cpp" file, include the logo header file using the appropriate path. For example, #include "../Logo/logo.h". (A single dot (".") refers to the current directory.
	Two consecutive dots ("..") refer to the parent directory).

Excellent! Now your file should have a similar structure to the one shown below. Please note that I've significantly reduced the number of bytes for brevity.

logo.h:
```c++
#pragma once

unsigned char eclipse_logo[2815] = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0xC2,
	0x08, 0x03, 0x00, 0x00, 0x00, 0x7A, 0xCC, 0x57, 0x5C, 0x00, 0x00, 0x00,
	0xDB, 0x50, 0x4C, 0x54, 0x45, 0xFF, 0xFF, 0xFF, 0x2A, 0x1E, 0x56, 0xFF,
	0x95, 0x00, 0x25, 0x18, 0x53, 0x10, 0x00, 0x4A, 0xC2, 0xC0, 0xCC, 0x4B,
	0x42, 0x6E, 0xBE, 0xBB, 0xC9, 0x2C, 0x20, 0x59, 0x26, 0x18, 0x57, 0x64,
	0x5E, 0x82, 0x20, 0x08, 0x5C, 0x34, 0x26, 0x67, 0x2D, 0x1B, 0x65, 0x82,
	0x7B, 0x9F, 0xA5, 0xA0, 0xBB, 0x30, 0x19, 0x6D, 0x3C, 0x2B, 0x75, 0x19,
	0x01, 0x4D, 0x40, 0x2C, 0x7A, 0x44, 0x30, 0x80, 0xFF, 0x8D, 0x00, etc..
};
```

Now, let's begin with the first line of code, which should be declared near the top of the gui.cpp file. Referring to the code will help in better understanding its implementation.
```c++
IDirect3DTexture9* eclipseLogo = nullptr;
```

Next, we can define the size of our image by specifying the width and height parameters.
```c++
ImVec2 image_size(655, 70);
```

Now, we will proceed to load the image bytes into a texture. We will place this code within our previously created ImGui Render function (make this yourself). If needed, you can refer to the code for an example on the Render Function.
```c++
if (single_creation) {
	D3DXCreateTextureFromFileInMemoryEx(gui::device, &eclipse_logo, sizeof(eclipse_logo), 400, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &eclipseLogo);
	single_creation = false;
}
```

The purpose of this if-statement is to prevent the function from repeatedly creating the texture. By setting the single_creation boolean variable to false after creating the texture, we ensure that the texture is not created again, thereby preventing potential lag or crashes in our menu. You can observe the impact by removing this if-statement and observing the behavior ;D

Please ensure that you place the following line of code **outside** the ImGui Render function:
```c++
bool single_creation = true;
```

Lastly, we can render the image in our menu by adding the following line to the ImGui Render function.
```c++
ImGui::Image(eclipseLogo, image_size);
```

I hope this guide has provided you with clarity on how to implement this system into your menu. I highly recommend reviewing the complete code in your IDE as it will facilitate understanding. If you encounter any difficulties, don't hesitate to reach out to me on Discord using the provided information. I'm available to assist you at any time.

Enjoy your coding journey and happy coding!

**estetik**
