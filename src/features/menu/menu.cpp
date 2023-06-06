#include "menu.h"

#include <string>
#include <imgui/imgui.h>

#include <globals.h>

#include "tools/color.h"

#define COLOR_EDIT_FLAGS ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview
#define COLOR_PICKER_FLAGS 
#define TUBS_NUM 7
#define TAB_SIZE_W 127.f
#define TAB_SIZE_H 50.f
#define CHILDE_SIZE_W 330.f
#define TABS_MARGIN 6.f
#define CHILD_MARGIN 8.f
#define TOGGLE_BUTTON_SIZE ImVec2(45, 14)

enum class tabs
{
	legit_bot,
	rage,
	visuals,
	props,
	color,
	misc,
	settings
};

static const char* tabs_names[] = {
	"LEGITBOT##menubutton",
	"RAGE##menubutton",
	"VISUALS##menubutton",
	"PROPS##menubutton",
	"COLOR##menubutton",
	"MISC##menubutton",
	"SETTINGS##menubutton"
};

tabs current_tab = tabs::visuals;

Menu* menu = new Menu();


void Menu::draw()
{

}
