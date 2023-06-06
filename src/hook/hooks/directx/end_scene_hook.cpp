#include "end_scene_hook.h"
#include <hook/hooks_manager.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx9.h>

#include <tools/render_tool.h>

#include <globals.h>
#include <features/menu/menu.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool is_init = false;
static WNDPROC wnd_proc = nullptr;

LRESULT CALLBACK my_wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (wParam == WM_KEYDOWN) {
        if (lParam == settings::misc->menukey) {
            menu->is_open = !menu->is_open;
        }
    }

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return 1L;

    return CallWindowProc(wnd_proc, hWnd, msg, wParam, lParam);
}

void init(IDirect3DDevice9* device)
{
    ImGui::CreateContext();

    auto game_hwnd = FindWindowW(L"Valve001", nullptr);
    if (game_hwnd != NULL)
    {
        ImGui_ImplWin32_Init(game_hwnd);
        ImGui_ImplDX9_Init(device);

        ImGui::GetIO().IniFilename = nullptr;
        ImGui::StyleColorsDark();

        wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, LONG_PTR(my_wndproc)));

        render_tool->init(device);

        //RenderTool->create_im_font(menu->menu_font, "C:\\Windows\\Fonts\\Corbel.ttf", 30);

        menu->menu_button_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 35.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        menu->menu_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Segoeuib.ttf", 14.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
    	
        Interfaces->engine->get_screen_size(globals->screen_width, globals->screen_height);

        is_init = true;
    }
}

long __stdcall end_scane_hook::hook(IDirect3DDevice9* device)
{
    auto ret = HooksManager->end_scane_original(device);

    int w, h;

    Interfaces->engine->get_screen_size(w, h);

    if (w != globals->screen_width || h != globals->screen_height)
        render_tool->init(device);

    globals->screen_width = w;
    globals->screen_height = h;

    if (!is_init)
        init(device);

    IDirect3DStateBlock9* pixel_state = NULL;
    IDirect3DVertexDeclaration9* vertex_declaration;
    IDirect3DVertexShader9* vertex_shader;


    device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
    device->GetVertexDeclaration(&vertex_declaration);
    device->GetVertexShader(&vertex_shader);

    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	
    render_tool->render_frame();

    pixel_state->Apply();
    pixel_state->Release();
    device->SetVertexDeclaration(vertex_declaration);
    device->SetVertexShader(vertex_shader);
	
    return ret;
}
