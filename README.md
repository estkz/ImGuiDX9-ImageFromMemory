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

Now these are all the lines of code we are going to be implementing into our menu. But firstly, let's open main.cpp!

Step 1. We must be sure we are running this project in Release -> X86 | Also, we must ensure we are using the correct project configuration. Here is a list with all the important aspects:

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

Cool! Now your file should look somewhat similar to this (NOTE: I decreased the bytes by a lot, to make this REAMDE shorter).

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

Now we can start with the first line of code. We declare it near the top of the gui.cpp file. Please refer to the code, as it will make it simpler to understand!
```c++
IDirect3DTexture9* eclipseLogo = nullptr;
```

Next up we can define the size of our image. We use the parameters Width, followed by height.
```c++
ImVec2 image_size(655, 70);
```

Now we are going to load the image bytes into a texture. We place this code in our ImGui Render function which we've created before. 
(this is something you made yourself, you can take a look at the code if you want an example).
```c++
if (single_creation) {
D3DXCreateTextureFromFileInMemoryEx(gui::device, &eclipse_logo, sizeof(eclipse_logo), 400, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &eclipseLogo);
single_creation = false;
}
```
The idea behind this if-statement is to avoid this function from creating the texture again over and over again. With this trick we set the single_creation boolean variable to false once it's done with creating the texture. The texture will no longer create itself again, so our menu will not lag/crash. You can see for yourself what happens if you remove this if statement ;) 

Be sure to add the following line of code **outside** of the ImGui Render function:
```c++
bool single_creation = true;
```
And finally, we can render the image in our menu with the last line. We add this line to the ImGui Render function of course!
```c++
ImGui::Image(eclipseLogo, image_size);
```
