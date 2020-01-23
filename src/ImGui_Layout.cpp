#include "ImGui_Layout.h"

// Is a MonoWindow being constructed?
bool bMonoWindow = false;

ImVec2 WindowStartSize = { -1.f, -1.f };

ImVec2 PrevWindowPos = { -1.f, -1.f };
ImVec2 NextWindowPos = { -1.f, -1.f };

ImVec2 PrevWindowSize = { -1.f, -1.f };

ImVec2 NextWindowMaxSize = { -1.f, -1.f };
ImVec2 NextWindowMinSize = { -1.f, -1.f };

ImVec2 LayoutMinSize = { 0.f, 0.f };

namespace ImGui_Layout
{
	void BeginMonoWindow(const char *Name)
	{
		IM_ASSERT(bMonoWindow == false && "MonoWindow already being constructed!");

		ImGuiWindowFlags DesktopWindowFlags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove;

		auto a = ImGui::GetContentRegionAvail();

		bMonoWindow = true;
		ImGui::SetNextWindowSize(a);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		
		ImGui::Begin(Name, nullptr, DesktopWindowFlags);

		ImGui::PopStyleVar(2);
	}

	void EndMonoWindow()
	{
		bMonoWindow = false;
		ImGui::End();
	}

	void BeginLayout()
	{
		static bool RunOnce = false;

		if (!RunOnce)
		{
			WindowStartSize = ImGui::GetIO().DisplaySize;
			RunOnce = true;
		}

		PrevWindowPos = { -1.f, -1.f };
		PrevWindowSize = { -1.f, -1.f };
		LayoutMinSize = { 0.f, 0.f };
	}

	void _BeginAnchorWindow(const char* Name, ImVec2 Pos, ImVec2 Size, AnchorPoints Anchors)
	{
		ImGuiWindowFlags WindowFlags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove;

		ImGuiIO& IO = ImGui::GetIO();

		/*
			Window size
		*/
		// PIXEL SIZE
		ImVec2 WindowSize = { 0.f, 0.f };
		// X
		if (Anchors.Left && Anchors.Right)
		{
			WindowSize.x = (Size.x * IO.DisplaySize.x);
		}
		else
		{
			WindowSize.x = (Size.x * WindowStartSize.x);
		}
		// Y
		if (Anchors.Top && Anchors.Bottom)
		{
			WindowSize.y = (Size.y * IO.DisplaySize.y);
		}
		else
		{
			WindowSize.y = (Size.y * WindowStartSize.y);
		}

		/*
			Window position
		*/
		// PIXEL POSITION
		ImVec2 WindowPos = { 0.f, 0.f };

		
		//NextWindowPos = { -1.f, -1.f };

		// X
		if (NextWindowPos.x != -1.f)
		{
			WindowPos.x = (NextWindowPos.x * IO.DisplaySize.x);
			NextWindowPos.x = -1.f;
		}
		else if (Anchors.Left || (!Anchors.Left && !Anchors.Right))
		{
			WindowPos.x = (Pos.x * IO.DisplaySize.x);
		}
		else if (Anchors.Right)
		{
			WindowPos.x = Pos.x + (IO.DisplaySize.x - WindowStartSize.x);
		}
		// Y
		if (NextWindowPos.y != -1.f)
		{
			WindowPos.y = (NextWindowPos.y * IO.DisplaySize.y);
			NextWindowPos.y = -1.f;
		}
		else if (Anchors.Top)
		{
			WindowPos.y = Pos.y;
		}
		else
		{
			WindowPos.y = Pos.y + (IO.DisplaySize.y - WindowStartSize.y);
		}

		PrevWindowPos = WindowPos;
		PrevWindowSize = WindowSize;

		ImGui::SetNextWindowSize(WindowSize);
		ImGui::SetNextWindowPos(WindowPos);
		
		ImGui::Begin(Name, nullptr, WindowFlags);
	}

	void BeginAnchorWindow(const char* Name, ImVec2 TopLeft, ImVec2 BottomRight, ImGuiWindowFlags WindowFlags)
	{
		WindowFlags |=
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove;

		/*ImGuiWindowFlags WindowFlags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove;*/

		ImGuiIO& IO = ImGui::GetIO();
		IM_ASSERT(TopLeft.x <= BottomRight.x);
		IM_ASSERT(TopLeft.y < BottomRight.y);

		/*
			Window position
		*/
		if (NextWindowPos.x != -1.f)
		{
			TopLeft.x = NextWindowPos.x;
			NextWindowPos.x = -1.f;
		}
		ImVec2 WindowAbsPos = TopLeft;
		PrevWindowPos = WindowAbsPos;
		ImVec2 WindowPos = { WindowAbsPos.x * IO.DisplaySize.x, WindowAbsPos.y * IO.DisplaySize.y };

		/*
			Window size
		*/
		ImVec2 WindowAbsSize = { BottomRight.x - TopLeft.x, BottomRight.y - TopLeft.y };

		if (NextWindowMaxSize.x != -1.f && (WindowAbsSize.x * IO.DisplaySize.x) > NextWindowMaxSize.x)
		{
			WindowAbsSize.x = NextWindowMaxSize.x / IO.DisplaySize.x;
		}
		if (NextWindowMinSize.x != -1.f)
		{
			LayoutMinSize.x += NextWindowMinSize.x;	// Accumulate minimum widths

			if((WindowAbsSize.x * IO.DisplaySize.x) < NextWindowMinSize.x)
				WindowAbsSize.x = NextWindowMinSize.x / IO.DisplaySize.x;
		}
		NextWindowMaxSize.x = -1.f;
		NextWindowMinSize.x = -1.f;

		PrevWindowSize = WindowAbsSize;

		float WindowSizeX = WindowAbsSize.x * IO.DisplaySize.x;
		ImVec2 WindowSize = { WindowSizeX, WindowAbsSize.y * IO.DisplaySize.y };
		

		ImGui::SetNextWindowSize(WindowSize);
		ImGui::SetNextWindowPos(WindowPos);
		
		ImGui::Begin(Name, nullptr, WindowFlags);
	}

	void EndAnchorWindow()
	{
		ImGui::End();
	}

	void NextWindowHorizontalAlign()
	{
		//ImGuiIO& IO = ImGui::GetIO();
		NextWindowPos.x = (PrevWindowPos.x + PrevWindowSize.x);
	}

	void NextWindowWidthBounds(float Max, float Min)
	{
		if(Max != -1.f)
			NextWindowMaxSize.x = Max;
		if (Min != -1.f)
			NextWindowMinSize.x = Min;
	}

	void NextWindowWidth(float Width)
	{
		if (Width != -1.f)
		{
			NextWindowMaxSize.x = Width;
			NextWindowMinSize.x = Width;
		}
	}

	const ImVec2& MinimumDisplaySize()
	{
		return LayoutMinSize;
	}
}