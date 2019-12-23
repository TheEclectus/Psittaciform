#include "MainUI.h"

#include <string>

#include "ImGui/imgui_stdlib.h"

static std::string Buffer = "shish";

void MainUI()
{
	ImGuiIO &IO = ImGui::GetIO();

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize(IO.DisplaySize);
	ImGuiWindowFlags MainWindowFlags =
		//ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::Begin("MainWindow", nullptr, MainWindowFlags);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputTextMultiline("", &Buffer, ImVec2(0,0), ImGuiInputTextFlags_ReadOnly);
	ImGui::End();
	ImGui::PopStyleVar();
}