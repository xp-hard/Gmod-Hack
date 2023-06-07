#pragma once
#include <game_sdk/interfaces/include_interfaces.h>
#include <tools/mem_tools.h>
#include <d3d9.h>

// directx hook
struct directx_hook123 {
	WNDCLASSEX WindowClass;
	HWND WindowHwnd;

	bool InitWindow() {

		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = "MJ";
		WindowClass.hIconSm = NULL;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == NULL) {
			return false;
		}
		return true;
	}

	bool DeleteWindow() {
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) {
			return false;
		}
		return true;
	}

	bool Init(LPDIRECT3DDEVICE9 Device) {
		if (InitWindow() == false) {
			return false;
		}

		HMODULE D3D9Module = GetModuleHandle("d3d9.dll");
		if (D3D9Module == NULL) {
			DeleteWindow();
			return false;
		}

		void* Direct3DCreate9 = GetProcAddress(D3D9Module, "Direct3DCreate9");
		if (Direct3DCreate9 == NULL) {
			DeleteWindow();
			return false;
		}

		LPDIRECT3D9 Direct3D9 = ((LPDIRECT3D9(__stdcall*)(uint32_t))(Direct3DCreate9))(D3D_SDK_VERSION);
		if (Direct3D9 == NULL) {
			DeleteWindow();
			return false;
		}

		D3DDISPLAYMODE DisplayMode;
		if (Direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode) < 0) {
			DeleteWindow();
			return false;
		}

		D3DPRESENT_PARAMETERS Params;
		Params.BackBufferWidth = 0;
		Params.BackBufferHeight = 0;
		Params.BackBufferFormat = DisplayMode.Format;
		Params.BackBufferCount = 0;
		Params.MultiSampleType = D3DMULTISAMPLE_NONE;
		Params.MultiSampleQuality = NULL;
		Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		Params.hDeviceWindow = WindowHwnd;
		Params.Windowed = 1;
		Params.EnableAutoDepthStencil = 0;
		Params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		Params.Flags = NULL;
		Params.FullScreen_RefreshRateInHz = 0;
		Params.PresentationInterval = 0;

		if (Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, WindowHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &Params, &Device) < 0) {
			Direct3D9->Release();
			DeleteWindow();
			return false;
		}
	}
};

#define INTERFACES_INTERNAL

#ifdef INTERFACES_INTERNAL
#define ADD_INTERFACE(varname, interface_type) interface_type* varname = nullptr;
#define INIT_INTERFACE(varname, interface_type, i_module, i_name) varname = (interface_type*)(MemTools::get_interface(i_module, i_name));
#endif

class CInterfaces {
public:
	ADD_INTERFACE(client, VClient);
	ADD_INTERFACE(engine, VEngineClient);
	ADD_INTERFACE(client_mode, IClientMode);
	ADD_INTERFACE(entity_list, VClientEntityList);
	ADD_INTERFACE(directx_device, IDirect3DDevice9);
	ADD_INTERFACE(material_system, IMaterialSystem);
	ADD_INTERFACE(view_render, IViewRender);
	ADD_INTERFACE(render_view, VEngineRenderView);
	ADD_INTERFACE(render, IRender);
	ADD_INTERFACE(model_render, IVModelRender);
	ADD_INTERFACE(surface, ISurface);
	ADD_INTERFACE(lua_shared, LUA::Shared);
	ADD_INTERFACE(engine_trace, IEngineTrace);
	ADD_INTERFACE(render_context, IMatRenderContext);
public:
	void init();

	void print_interfaces();
};

extern CInterfaces* Interfaces;

#undef INTERFACES_INTERNAL