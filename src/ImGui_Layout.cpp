#include "ImGui_Layout.h"

#include <ImGui/imgui_internal.h>

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

namespace ImGui_Props
{
	bool bPropertyGrid = false;
	bool bHasHorizontalDividers = false;
	bool bGridSizeable = false;
	const char* GridID = nullptr;

	size_t GridItemIndex = 0u;

	void SetupColumns()
	{
		float Indent = ImGui::GetCurrentWindow()->DC.Indent.x;
		float AvailW = ImGui::GetContentRegionAvail().x;

		float IndentedAvailW = AvailW - Indent;

		ImGui::Columns(2, GridID, bGridSizeable);
		//ImGui::SetColumnOffset(0, Indent);
		ImGui::SetColumnWidth(0, (AvailW * 0.5f) + (Indent * 0.5f));
	}

	void GridItemBegin(const std::string &Label)
	{
		IM_ASSERT(bPropertyGrid && "PropertyGridInline* functions must be called between BeginPropertyGrid() and EndPropertyGrid().");
		ImGui::PushID(GridItemIndex);

		ImGui::AlignTextToFramePadding();
		ImGui::Text(Label.c_str());
		ImGui::NextColumn();
	}

	void GridItemEnd()
	{
		ImGui::NextColumn();
		ImGui::PopID();
		GridItemIndex++;

		if (bHasHorizontalDividers)
			ImGui::Separator();
	}

	void BeginPropertyGrid(const char* ID, bool bSizeable, bool bHasDividers)
	{
		IM_ASSERT(bPropertyGrid == false && "Property grid already being constructed!");

		bPropertyGrid = true;
		GridID = ID;
		bHasHorizontalDividers = bHasDividers;
		bGridSizeable = bSizeable;

		SetupColumns();

		ImGui::PushID(ID);
		GridItemIndex = 0u;
	}

	void EndPropertyGrid()
	{
		ImGui::Columns();

		bPropertyGrid = false;
		GridID = nullptr;
		bHasHorizontalDividers = false;
		bGridSizeable = false;

		ImGui::PopID();
	}

	/*template<class T, size_t Spare>
	void PropertyGridInlineEntry(T Var)
	{
		IM_ASSERT(false && "Invalid type for inline edit field.");
	}*/

	bool BeginCollapsingRegion(const std::string& Label)
	{
		ImGui::Columns();
		bool ToReturn = ImGui::CollapsingHeader(Label.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

		if (bHasHorizontalDividers)
			ImGui::Separator();

		ImGui::Indent();

		SetupColumns();

		return ToReturn;
	}

	void EndCollapsingRegion()
	{
		ImGui::Unindent();

		// Programmers hate this one neat trick to get the columns to format correctly again!
		ImGui::Columns();
		SetupColumns();

		//ImGui::TreePop();
	}

	bool PropertyGridTextInput(const std::string& Label, char* Buffer, size_t BufLen, ImGuiInputTextFlags Flags, ImGuiInputTextCallback Callback, void* UserData)
	{
		GridItemBegin(Label);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool ToReturn = ImGui::InputText("", Buffer, BufLen, Flags, Callback, UserData);

		GridItemEnd();
		return ToReturn;
	}

	bool PropertyGridIntInput(const std::string& Label, int& Value, const int Min, const int Max, const char* Prefix, const char* Postfix)
	{
		IM_ASSERT(Min < Max && "PropertyGridIntInput(): Min param must be < Max param.");

		GridItemBegin(Label);

		if (Value < Min)
			Value = Min;
		if (Value > Max)
			Value = Max;

		std::string FmtString = "%d";
		if (Prefix)		FmtString = Prefix + FmtString;
		if (Postfix)	FmtString += Postfix;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool ToReturn = ImGui::InputScalar("", ImGuiDataType_S32, &Value, nullptr, nullptr, FmtString.c_str());

		GridItemEnd();
		return ToReturn;
	}

	bool PropertyGridFloatInput(const std::string& Label, float& Value, bool* bShowDecimalPlaces, uint8_t* DecimalPlaces, const float Min, const float Max, const char* Prefix, const char* Postfix)
	{
		IM_ASSERT(Min < Max && "PropertyGridFloatInput(): Min param must be < Max param.");

		GridItemBegin(Label);

		if (Value < Min)
			Value = Min;
		if (Value > Max)
			Value = Max;

		bool ToReturn = false;
		if (!bShowDecimalPlaces)
		{
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

			std::string FmtString = "%.2f";
			if (Prefix)		FmtString = Prefix + FmtString;
			if (Postfix)	FmtString += Postfix;
			ToReturn = ImGui::InputScalar("", ImGuiDataType_Float, &Value, nullptr, nullptr, FmtString.c_str());
		}
		else
		{
			IM_ASSERT(bShowDecimalPlaces && DecimalPlaces);

			if (*DecimalPlaces < 0)
				*DecimalPlaces = 0;
			if (*DecimalPlaces > 4)
				*DecimalPlaces = 4;

			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, ImGui::GetStyle().ItemSpacing.y });

			std::string FmtString = std::string("%.") + std::to_string(*DecimalPlaces) + "f";
			if (Prefix)		FmtString = Prefix + FmtString;
			if (Postfix)	FmtString += Postfix;

			float AvailW = ImGui::GetContentRegionAvail().x;

			float InputW = (*bShowDecimalPlaces ? (AvailW * 0.8f) : AvailW);
			ImGui::SetNextItemWidth(InputW);

			ToReturn = ImGui::InputScalar("", ImGuiDataType_Float, &Value, nullptr, nullptr, FmtString.c_str());
			/*ImGui::SameLine();
			if (ImGui::ArrowButton("showhidedecs", (*bShowDecimalPlaces ? ImGuiDir_Right : ImGuiDir_Left)))
			{
				*bShowDecimalPlaces = !*bShowDecimalPlaces;
			}*/

			if (*bShowDecimalPlaces)
			{
				static const int DragMin = 0, DragMax = 4;

				float DecW = (AvailW * 0.2f) - ImGui::GetStyle().ItemSpacing.x;

				ImGui::SameLine();
				ImGui::PushID("DecimalPlaces");
				ImGui::SetNextItemWidth(DecW);
				ImGui::DragScalar("", ImGuiDataType_U8, DecimalPlaces, 0.1f, &DragMin, &DragMax);
				//ImGui::InputScalar("", ImGuiDataType_U8, DecimalPlaces);
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Decimal Places");
				}
				ImGui::PopID();
			}
			

			//ImGui::PopStyleVar();
		}

		GridItemEnd();
		return ToReturn;
	}

	bool PropertyGridCombo(const std::string& Label, int* CurrentItem, const char* const Items[], size_t ItemCount)
	{
		GridItemBegin(Label);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool ToReturn = ImGui::Combo("", CurrentItem, Items, ItemCount);

		GridItemEnd();
		return ToReturn;
	}
}