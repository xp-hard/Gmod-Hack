#include "interfaces.h"
#include <tools/logger.h>

template<typename return_type, typename address_type = uintptr_t, typename offset_type = uint32_t>
constexpr auto relative_to_absolute(address_type address, offset_type offset, auto instruction_size) noexcept {
	return reinterpret_cast<return_type*>(address
		+ instruction_size
		+ (*reinterpret_cast<offset_type*>(address + offset)));
}

CInterfaces* Interfaces = new CInterfaces();

static inline void print_interface(const std::string& name, DWORD address) { Logger->message(std::string(name + "\t") + std::to_string(address)); }

directx_hook123 directx_hook;

void CInterfaces::init()
{
	INIT_INTERFACE(client, VClient, "client.dll", "VClient017");
	INIT_INTERFACE(entity_list, VClientEntityList, "client.dll", "VClientEntityList003");

	INIT_INTERFACE(engine, VEngineClient, "engine.dll", "VEngineClient015");
	INIT_INTERFACE(render_view, VEngineRenderView, "engine.dll", "VEngineRenderView014");
	INIT_INTERFACE(model_render, IVModelRender, "engine.dll", "VEngineModel016");
	INIT_INTERFACE(engine_trace, IEngineTrace, "engine.dll", "EngineTraceClient003");

	INIT_INTERFACE(material_system, IMaterialSystem, "materialsystem.dll", " VMaterialSystem080");

	INIT_INTERFACE(surface, ISurface, "vguimatsurface", "VGUI_Surface030");

	INIT_INTERFACE(lua_shared, LUA::Shared, "lua_shared.dll", "LUASHARED003");
	/*
	do {
		auto address = (uintptr_t)MemTools::pattern_scaner("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 48 FF 60 50 CC CC 48 83 EC 28");
		auto address_of_next = address + 7;
		auto relative = *(int*)(address + 3);
		this->client_mode = (IClientMode*)(relative + address_of_next);
	} while (!this->client_mode);
	*/
	do {
		this->client_mode = relative_to_absolute<IClientMode>(
			(uintptr_t)MemTools::pattern_scaner("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 48 FF 60 50 CC CC 48 83 EC 28"),
			7,
			3);
	} while (!this->client_mode);

	directx_hook.Init(directx_device);

	//view_render = **(IViewRender***)((*(uintptr_t**)client)[27] + 0x5);

	view_render = relative_to_absolute<IViewRender>((*((uintptr_t**)(client)))[27], 7, 3);

	render = relative_to_absolute<IRender>((*((uintptr_t**)(render_view)))[12], 7, 3);

	render_context = material_system->GetRenderContext();
}

void CInterfaces::print_interfaces()
{
	print_interface("Client", (DWORD)client);
	print_interface("ClientMode", (DWORD)client_mode);
	print_interface("ClientEntityList", (DWORD)entity_list);
	print_interface("ViewRender", (DWORD)view_render);

	print_interface("Engine", (DWORD)engine);
	print_interface("RenderView", (DWORD)render_view);
	print_interface("IRender", (DWORD)render);
	print_interface("ITrace", (DWORD)engine_trace);

	print_interface("Direct3DDevice", (DWORD)directx_device);

	print_interface("MaterialSystem", (DWORD)material_system);

	print_interface("Surface", (DWORD)surface);
}
