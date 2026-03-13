# 💻 ImGuiDX9 – Image From Memory

A modern, lightweight example showing how to **load image data directly from memory into a DirectX9 texture for Dear ImGui**, without using the legacy DirectX SDK.

This project demonstrates the **modern approach** using the widely adopted `stb_image` library instead of deprecated DirectX helpers like `D3DXCreateTextureFromFileInMemory`.

---

# 🌒 About

Older tutorials rely on:

```
D3DXCreateTextureFromFileInMemory
```

which requires the **deprecated Microsoft DirectX SDK**.

This repository demonstrates the **modern workflow**:

1. **Decode image bytes** (PNG/JPG/etc.) using `stb_image`
2. **Create a DirectX9 texture**
3. **Manually upload pixels to GPU memory**
4. **Convert RGBA → BGRA** to match DirectX9's expected format

This approach is:

• ✔ Portable (no legacy SDK required)  
• ✔ Lightweight (single-header image library)  
• ✔ Modern and maintainable  

---

# ⚙️ Requirements

### IDE
Visual Studio **2022 recommended**

### Build Configuration

```
Release | x86
```

### Dependencies

Included in the repository:

- Dear ImGui
- stb_image (header only)

### DirectX

Uses the **standard Windows SDK**

No additional DirectX SDK installation required.

---

# 📁 Project Structure

Example layout used in this repository:

```
project/
│
├─ imgui/
├─ third_party/
│   └─ stb/
│
├─ logo/
│   └─ logo.h
│
├─ gui/
│   ├─ gui.cpp
│   └─ gui.h
│
└─ main.cpp
```

---

# 📃 Data → Image Guide

## 1. Convert an Image to C Bytes

Use a hex editor such as **HxD** or an online tool like:

https://hexed.it/

### Using HxD

1. Download HxD  
https://mh-nexus.de/en/hxd/

2. Open your image file

3. Go to:

```
File → Export → C
```

4. Save the exported file

5. Open the file in Notepad

6. Copy the generated array

Example:

```cpp
unsigned char rawData[12345] = { 0x89, 0x50, 0x4E, ... };
```

7. Create a header file:

```
logo.h
```

Example:

```cpp
#pragma once

unsigned char eclipse_logo[] = {
    0x89, 0x50, 0x4E, ...
};
```

---

# 2. Include the Image Header

Inside your source file:

```cpp
#include "../logo/logo.h"
```

Directory reference reminder:

```
.   = current directory
..  = parent directory
```

---

# 3. Add the Texture Loader

You must include the function responsible for converting image bytes into a DirectX9 texture.

This function:

1. Decodes PNG/JPG bytes using `stb_image`
2. Creates a DirectX texture
3. Converts RGBA pixels → BGRA
4. Uploads the image to GPU memory

Function signature:

```cpp
bool LoadTextureFromMemory(
    IDirect3DDevice9* device,
    const unsigned char* data,
    int size,
    IDirect3DTexture9** outTexture
);
```

The full implementation is included in `gui.cpp`.

---

# 4. Initialize the Texture

Load the texture **once during initialization**.

Example (inside your ImGui setup):

```cpp
static IDirect3DTexture9* eclipseLogo = nullptr;

LoadTextureFromMemory(
    device,
    eclipse_logo,
    sizeof(eclipse_logo),
    &eclipseLogo
);
```

⚠ Important

Do **NOT** load the texture every frame.

Doing so will cause:

- GPU memory leaks
- Severe performance loss

---

# 5. Render the Image

Use the ImGui image widget:

```cpp
ImVec2 size = ImVec2(200, 75);

ImGui::Image(
    (void*)eclipseLogo,
    size
);
```

Example with tint / alpha / scaling:

```cpp
static float logo_alpha = 1.0f;
static float logo_tint[4] = {1.0f,1.0f,1.0f,1.0f};
static float logo_scale = 1.0f;

ImVec2 size = ImVec2(200 * logo_scale, 75 * logo_scale);

ImGui::Image(
    (void*)eclipseLogo,
    size,
    ImVec2(0,0),
    ImVec2(1,1),
    ImVec4(logo_tint[0],logo_tint[1],logo_tint[2],logo_alpha)
);
```

---

# 🎨 Why RGBA → BGRA Conversion Is Needed

`stb_image` outputs pixels in:

```
RGBA
```

DirectX9 expects textures in:

```
BGRA
```

Therefore we manually swap the red and blue channels:

```cpp
pDestRow[x * 4 + 0] = pSrcRow[x * 4 + 2]; // Blue
pDestRow[x * 4 + 1] = pSrcRow[x * 4 + 1]; // Green
pDestRow[x * 4 + 2] = pSrcRow[x * 4 + 0]; // Red
pDestRow[x * 4 + 3] = pSrcRow[x * 4 + 3]; // Alpha
```

Without this conversion your image will display with **incorrect colors**.

---

# 🧹 Cleanup

Always release the texture during shutdown.

Example:

```cpp
if (eclipseLogo)
{
    eclipseLogo->Release();
    eclipseLogo = nullptr;
}
```

Failing to release textures will cause **GPU memory leaks**.

---

# 🖼 Supported Image Formats

`stb_image` supports:

- PNG
- JPG
- BMP
- TGA
- PSD
- GIF
- HDR

Recommended maximum texture size:

```
4096 x 4096
```

(Depends on GPU capabilities)

---

# 🛠 Technical Features

• No `d3dx9.h` dependency  
• Uses modern Windows SDK only  
• Header-only image decoding  
• Direct GPU upload  
• Simple demo UI for experimenting with image properties  

---

# 📚 Minimal Integration Example

A minimal usage example:

```cpp
IDirect3DTexture9* logo = nullptr;

LoadTextureFromMemory(device, logo_bytes, sizeof(logo_bytes), &logo);

ImGui::Begin("Example");

ImGui::Image((void*)logo, ImVec2(200,75));

ImGui::End();
```

---

# 💎 Contact

You can reach me on Discord:

```
estkz
e9r
```

---

# 📄 License

This project uses:

- Dear ImGui (MIT License)
- stb_image (Public Domain / MIT)

Refer to their respective repositories for license details.

---

I hope this guide has provided you with clarity on how to implement this system into your menu. I highly recommend reviewing the complete code in your IDE as it will help with understanding. If you encounter any difficulties, don't hesitate to reach out to me on Discord using the provided information. I'm available to assist you at any time.

Happy coding 🚀