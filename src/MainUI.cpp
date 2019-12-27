#include "MainUI.h"

#include <algorithm>
#include <string>
#include <vector>

#include "ImGui/imgui_stdlib.h"

#include "Script.h"

static std::string LogBuffer = "shish";
static std::string InputBuffer = "";

static std::vector<Script> Scripts;

void Setup()
{
	Scripts.push_back(Script("Scrape Site"));
	Scripts.push_back(Script("Mass-Rename Files"));
	Scripts.push_back(Script("Empty Recycle Bin"));
}

void MainUI()
{
	ImGuiIO &IO = ImGui::GetIO();
	ImGuiStyle& Style = ImGui::GetStyle();

	ImVec2 MenuSize(0.f, 0.f);
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Test"))
			{

			}
			ImGui::EndMenu();
		}

		MenuSize = ImGui::GetWindowSize();
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowPos({ 0, MenuSize.y });
	float PaneHeight = IO.DisplaySize.y - MenuSize.y;
	ImGui::SetNextWindowSize({ IO.DisplaySize.x, PaneHeight });
	ImGuiWindowFlags MainWindowFlags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::Begin("MainWindow", nullptr, MainWindowFlags);
		ImGui::BeginChild(ImGui::GetID("LuaTerm"), ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, 0));
			ImGui::BeginGroup();
				ImGui::BeginTabBar("LuaTermTabs");
					if (ImGui::BeginTabItem(" * "))
					{
						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						ImGui::InputTextMultiline("LuaLog", &LogBuffer, ImVec2(0, -60), ImGuiInputTextFlags_None);

						if (ImGui::Button("Clear Log"))
						{

						}

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						//ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 30);
						//ImGui::InvisibleButton("aa", { ImGui::GetContentRegionAvail().x - 30, 0 });

						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						ImGui::InputText("LuaInput", &InputBuffer);
						ImGui::EndTabItem();
					}
				ImGui::EndTabBar();
			ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild(ImGui::GetID("Toolbar"), ImVec2(0,0));
			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
			if (ImGui::BeginTabBar("ToolbarTabs"))
			{
				//ImGui::PopStyleVar(1);
				ImGui::Spacing();
				if (ImGui::BeginTabItem("Scripts"))
				{
					static bool DetailView = false;
					ImGui::Checkbox("Detail View", &DetailView);
					//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8,0 });
					ImGui::BeginChild(ImGui::GetID("ScriptsTabContent"), ImVec2(0, 0), true);
						// One of these for each loaded script
						for (auto &CurScript : Scripts)
						{
							const std::string& ScriptName = CurScript.Name();

							if (!DetailView)
							{
								ImGui::Button(ScriptName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 30));
								
								//std::string MenuID = "Popup_" + CurScript.Name();

								
								if (ImGui::BeginPopupContextItem(ScriptName.c_str()))
								{
									if (ImGui::MenuItem("Edit"))
									{

									}
									if (ImGui::MenuItem("Console"))
									{

									}
									if (ImGui::MenuItem("Logs"))
									{

									}
									//if (ImGui::Selectable("Set to zero")) value = 0.0f;
									//if (ImGui::Selectable("Set to PI")) value = 3.1415f;
									//ImGui::SetNextItemWidth(-1);
									//ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
									ImGui::EndPopup();
								}
							}
							else
							{
								const std::string &ScriptDesc = CurScript.Description();
								ImGui::Text(ScriptName.c_str());
								ImGui::TextWrapped(ScriptDesc.c_str());

								ImGui::SmallButton("Run");
								ImGui::SameLine();
								ImGui::SmallButton("Edit");
								ImGui::SameLine();
								ImGui::SmallButton("Console");
								ImGui::SameLine();
								ImGui::SmallButton("Logs");

								ImGui::Separator();
							}
							

							//if (i > 0) ImGui::Separator();
							
							//std::string Text = "Demo Script";
							//ImGui::Text
							//ImGui::SmallButton("belp");
							
							/*auto TextSize = ImGui::CalcTextSize(Text.c_str(), nullptr, false, ImGui::GetContentRegionAvail().x);
							float ChildHeight = std::max(30.f, TextSize.y);
							float CenteredButtonY = (ChildHeight / 2.f) - (30.f / 2.f);
							float ChildOffset = 0;
							if (CenteredButtonY == 0)
								ChildOffset = 15 - (TextSize.y / 2);*/
							
							/*ImGui::BeginGroup();
								ImGui::BeginGroup();
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + CenteredButtonY + 4);
									ImGui::Button("GO", ImVec2(30, 30));
								ImGui::EndGroup();
								ImGui::SameLine();
								ImGui::BeginGroup();
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ChildOffset);
									ImGui::TextWrapped(Text.c_str());
								ImGui::EndGroup();
							ImGui::EndGroup();

							ImGui::Separator();*/

							
						}
						
					ImGui::EndChild();
					//ImGui::PopStyleVar();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Cron"))
				{
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			else
			{
				//ImGui::PopStyleVar(1);
			}
			
		ImGui::EndChild();
	ImGui::End();
	ImGui::PopStyleVar(2);
}