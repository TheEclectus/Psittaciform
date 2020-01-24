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
	void BeginPropertyGrid(const char* ID = nullptr, bool bSizeable = false, bool bHasDividers = false);
	void EndPropertyGrid();

	/*
		String entry
	*/
	template<size_t Len>
	void PropertyGridInlineEntry(char (&n)[Len]);
	void PropertyGridInlineEntry(char* Buffer, size_t BufLen);
}