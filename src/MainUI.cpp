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
		ImGui::BeginChild(ImGui::GetID("LuaTerm"), ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, 0));
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::InputTextMultiline("", &Buffer, ImVec2(0, 0), ImGuiInputTextFlags_None);
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild(ImGui::GetID("Toolbar"));
			ImGui::Text("Well, here's to us.");
		ImGui::EndChild();
	ImGui::End();
	ImGui::PopStyleVar();
}