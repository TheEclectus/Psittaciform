#pragma once

#include <string>

#include <ImGui/imgui.h>

/// <summary>
/// Layout the display window like a full desktop application.
/// </summary>

namespace ImGui_Layout
{
	// Desktop Window - fills the screen with 

	/// <summary>
	/// MonoWindow is a Window that fills the entire screen, cannot be moved and 
	/// won't be brought forward on focus.
	/// </summary>
	void BeginMonoWindow(const char *Name);
	void EndMonoWindow();

	struct AnchorPoints
	{
		bool	Left = true,	// X position pinned
				Right = true,	// X position moves with window size
				Top = true,		// Y position pinned
				Bottom = true;	// Y position moves with window size

		// Left + Right		-> Width is original X position + Right x position
		// Top + Bottom		-> Height is original Y position + Bottom y position
	};

	// Only needs to be called once, after ImGui::NewFrame().
	void BeginLayout();

	// Pos and Size are both percentile coordinates of available screenspace.
	//void BeginAnchorWindow(const char* Name, ImVec2 Pos, ImVec2 Size, AnchorPoints Anchors = AnchorPoints());
	void BeginAnchorWindow(const char* Name, ImVec2 TopLeft, ImVec2 BottomRight, ImGuiWindowFlags WindowFlags = 0);
	void EndAnchorWindow();

	void NextWindowHorizontalAlign();
	// In pixels
	void NextWindowWidthBounds(float Max = -1, float Min = -1);
	void NextWindowWidth(float Width = -1);	// Same as calling NextWidthBounds() with the same parameters

	const ImVec2 &MinimumDisplaySize();
}

namespace ImGui_Props
{
	void GridItemBegin(const std::string& Label);
	void GridItemEnd();

	void BeginPropertyGrid(const char* ID = nullptr, bool bSizeable = false, bool bHasDividers = false);
	void EndPropertyGrid();

	/*
		Collapsing Region
	*/
	bool BeginCollapsingRegion(const std::string& Label);
	void EndCollapsingRegion();

	/*
		Text Input (by default, returns true when the value has changed)
	*/
	template<size_t Len>
	bool PropertyGridTextInput(const std::string &Label, char(&n)[Len], ImGuiInputTextFlags Flags = 0, ImGuiInputTextCallback Callback = nullptr, void *UserData = nullptr)
	{
		return PropertyGridTextInput(Label, n, Len, Flags, Callback, UserData);
	}
	bool PropertyGridTextInput(const std::string& Label, char* Buffer, size_t BufLen, ImGuiInputTextFlags Flags = 0, ImGuiInputTextCallback Callback = nullptr, void* UserData = nullptr);

	/*
		Number Input
	*/
	bool PropertyGridIntInput(const std::string& Label, int& Value, const int Min = (std::numeric_limits<int>::min)(), const int Max = (std::numeric_limits<int>::max)(), const char *Prefix = nullptr, const char *Postfix = nullptr);
	bool PropertyGridFloatInput(const std::string& Label, float& Value, bool *bShowDecimalPlaces = nullptr, uint8_t *DecimalPlaces = nullptr, const float Min = (std::numeric_limits<float>::min)(), const float Max = (std::numeric_limits<float>::max)(), const char *Prefix = nullptr, const char *Postfix = nullptr);

	/*
		Combo Box
	*/
	bool PropertyGridCombo(const std::string& Label, int* CurrentItem, const char* const Items[], size_t ItemCount);
}