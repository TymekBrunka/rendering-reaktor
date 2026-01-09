#pragma once
#include "imgui.h"
#include "Texture2d.hpp"

namespace EditorActionsPanel {
	struct actionEntry {
		const char* label;
		ImVec2 begin;
		ImVec2 end;
    void (*callback)();
	};

	extern actionEntry entries[];

	void UI(RR::Texture2d& icon, RR::Texture2d& icons);
}
