#define NOMINMAX
#include <windows.h>
#include <shellapi.h>

#include "gui.h"

#include "../logo/logo.h" // Don't forget to include your logo header

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

// STB_IMAGE: A simple, one-file library that turns image files (PNG/JPG) 
// into raw pixel data so DirectX can display them.
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb/stb_image.h"

// This pointer will hold our final image once it is loaded into the GPU memory
static IDirect3DTexture9* eclipseLogo = nullptr;

static int active_tab = 0;

// Image properties
static float logo_alpha = 1.0f;
static float logo_tint[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static float logo_size_mult = 1.0f;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

/* CORE LOGIC: Converts raw bytes from memory into a DirectX Texture.
    1. Decodes the byte array (PNG/JPG) into raw pixels using STB.
    2. Creates an empty DirectX texture on the GPU.
    3. Manually copies and reorders pixels from RGBA to BGRA (DirectX 9 standard).
*/
bool LoadTextureFromMemory(IDirect3DDevice9* device, const unsigned char* data, int size, IDirect3DTexture9** outTexture)
{
    int width, height, channels;

    // Decodes image from memory buffer into a raw pixel array (RGBA)
    unsigned char* image = stbi_load_from_memory(data, size, &width, &height, &channels, 4);
    if (!image) return false;

    IDirect3DTexture9* texture = nullptr;

    // Creates a texture object in DirectX managed memory
    if (FAILED(device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, nullptr))) {
        stbi_image_free(image);
        return false;
    }

    D3DLOCKED_RECT rect;

    // Lock the texture to get direct access to its memory address
    if (SUCCEEDED(texture->LockRect(0, &rect, nullptr, 0))) {
        unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
        for (int y = 0; y < height; y++) {
            unsigned char* pSrcRow = image + (y * width * 4);
            unsigned char* pDestRow = dest + (y * rect.Pitch);
            for (int x = 0; x < width; x++) {
                pDestRow[x * 4 + 0] = pSrcRow[x * 4 + 2]; // B
                pDestRow[x * 4 + 1] = pSrcRow[x * 4 + 1]; // G
                pDestRow[x * 4 + 2] = pSrcRow[x * 4 + 0]; // R
                pDestRow[x * 4 + 3] = pSrcRow[x * 4 + 3]; // A
            }
        }
        texture->UnlockRect(0);
    }
    stbi_image_free(image);
    *outTexture = texture;
    return true;
}

LRESULT __stdcall WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message) {
    case WM_SIZE:
        if (gui::device && wideParameter != SIZE_MINIMIZED) {
            gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
            gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
            gui::ResetDevice();
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
    windowClass = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProcess, 0, 0, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, "software", nullptr };
    RegisterClassEx(&windowClass);
    window = CreateWindowEx(0, "software", windowName, WS_POPUP, 100, 100, WIDTH, HEIGHT, nullptr, nullptr, windowClass.hInstance, nullptr);
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
    if (!d3d) return false;
    ZeroMemory(&presentParameters, sizeof(presentParameters));
    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    return SUCCEEDED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device));
}

void gui::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    if (device->Reset(&presentParameters) == D3DERR_INVALIDCALL) IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
    if (eclipseLogo) { eclipseLogo->Release(); eclipseLogo = nullptr; }
    if (device) { device->Release(); device = nullptr; }
    if (d3d) { d3d->Release(); d3d = nullptr; }
}

void gui::CreateImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);

    // Call the texture loader during ImGui initialization!
    LoadTextureFromMemory(device, eclipse_logo, sizeof(eclipse_logo), &eclipseLogo);
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
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
        if (message.message == WM_QUIT) { isRunning = false; return; }
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
    ImGui::EndFrame();
    device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
    if (SUCCEEDED(device->BeginScene())) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }
    if (device->Present(nullptr, nullptr, nullptr, nullptr) == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}

void gui::Render() noexcept
{
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ static_cast<float>(WIDTH), static_cast<float>(HEIGHT) });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Dashboard", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PopStyleVar(2);

    ImGui::SetCursorPos({ 0, 0 });
    ImGui::InvisibleButton("##DragZone", ImVec2(static_cast<float>(WIDTH), 30));
    if (ImGui::IsItemActive()) {
        POINT mPos; GetCursorPos(&mPos);
        static POINT lastMPos = mPos;
        if (ImGui::IsMouseClicked(0)) lastMPos = mPos;
        RECT wRect; GetWindowRect(window, &wRect);
        SetWindowPos(window, nullptr, wRect.left + mPos.x - lastMPos.x, wRect.top + mPos.y - lastMPos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        lastMPos = mPos;
    }

    // Sidebar
    ImGui::SetCursorPos({ 0, 0 });
    ImGui::BeginChild("##Sidebar", ImVec2(130, static_cast<float>(HEIGHT)), true, ImGuiWindowFlags_NoScrollbar);

    ImGui::Spacing();
    ImGui::SetCursorPosX(10); ImGui::Text("IMAGE DEMO");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Selectable("  Dashboard", active_tab == 0)) active_tab = 0;
    if (ImGui::Selectable("  Settings", active_tab == 1)) active_tab = 1;

    ImGui::SetCursorPosY(static_cast<float>(HEIGHT) - 30);
    ImGui::Separator();
    if (ImGui::Selectable("  EXIT", false)) isRunning = false;

    ImGui::EndChild();

    ImGui::SameLine();

    // Content
    ImGui::SetCursorPosX(140);
    ImGui::SetCursorPosY(10);

    ImGui::BeginGroup();

    if (active_tab == 0) {
        ImGui::Text("IMAGE CONTROL CENTER");
        ImGui::Separator();

        ImGui::BeginChild("##Canvas", ImVec2(ImGui::GetContentRegionAvail().x - 10, 120), false, ImGuiWindowFlags_NoScrollbar);

        // Only render if the render is not a nullptr
        if (eclipseLogo) {
            /* 
                Rendering the Texture
                The ImGui::Image function takes the GPU texture pointer (eclipseLogo).
                We pass size, UV coordinates, and a tint color (logo_tint + logo_alpha).
            */
            ImVec2 size = ImVec2(200 * logo_size_mult, 75 * logo_size_mult);
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) * 0.5f);
            ImGui::SetCursorPosY((ImGui::GetWindowHeight() - size.y) * 0.5f);
            ImGui::Image((void*)eclipseLogo, size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(logo_tint[0], logo_tint[1], logo_tint[2], logo_alpha));
        }

        ImGui::EndChild();

        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(2, "##Controls", false);
        ImGui::SetColumnWidth(0, 160);

        ImGui::Text("Transparency");
        ImGui::PushItemWidth(-1);
        ImGui::SliderFloat("##Alpha", &logo_alpha, 0.0f, 1.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::Text("Scale Factor");
        ImGui::PushItemWidth(-1);
        ImGui::SliderFloat("##Scale", &logo_size_mult, 0.2f, 2.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        ImGui::Text("Color Filter");
        ImGui::ColorEdit3("##Tint", logo_tint, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

        ImGui::Spacing();
        if (ImGui::Button("Reset All", ImVec2(ImGui::GetContentRegionAvail().x - 10, 25))) {
            logo_alpha = 1.0f; logo_size_mult = 1.0f;
            logo_tint[0] = 1.0f; logo_tint[1] = 1.0f; logo_tint[2] = 1.0f;
        }
        ImGui::Columns(1);
    }
    else if (active_tab == 1) {
        ImGui::Text("SYSTEM OPTIONS");
        ImGui::Separator();
        ImGui::Spacing();

        static bool topMost = false;
        if (ImGui::Checkbox("Always on Top", &topMost)) {
            SetWindowPos(window, topMost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        ImGui::Spacing();
        if (ImGui::Button("GitHub Documentation", ImVec2(ImGui::GetContentRegionAvail().x - 10, 30))) {
            ShellExecuteA(0, "open", "https://github.com/estkz/ImGuiDX9-ImageFromMemory", 0, 0, SW_SHOW);
        }
    }

    ImGui::EndGroup();
    ImGui::End();
}