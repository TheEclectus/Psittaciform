#include <luajit.h>

#include <SDL.h>
#include <SDL_syswm.h>

#include <Windows.h>
#include <shellapi.h>

#include "glad/glad.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"

#include "MainUI.h"

#include <Sol2/sol.hpp>

//#include "LuaTask.h"

#include "ImGui_Layout.h"

#define winmsg (WM_USER + 1)
void TrayIcon(SDL_Window *Window, bool bAdd)
{
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);

	NOTIFYICONDATA icon;
	if (SDL_GetWindowWMInfo(Window, &info))
	{
		icon.uCallbackMessage = winmsg;
		icon.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		icon.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		icon.cbSize = sizeof(icon);
		icon.hWnd = info.info.win.window;
		lstrcpyW(icon.szTip, L"Test tip");

		if(bAdd)
			bool success = Shell_NotifyIcon(NIM_ADD, &icon);
		else
			bool success = Shell_NotifyIcon(NIM_DELETE, &icon);
	}
}

// Main code
int main(int, char**)
{
    //LuaTask* x = new LuaTask("C:\\Users\\Benjamin\\source\\repos\\Psittaciform\\bin\\Debug-x86\\test");

    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        return -1;
    }

	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Psittaciform", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 570, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    bool err = gladLoadGL() == 0;
    if (err)
    {
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Setup();

    char Buf[1024];
    memset(Buf, 0, sizeof(Buf));

    char DmgBuf[128];
    memset(DmgBuf, 0, sizeof(DmgBuf));

    // Main loop
    bool FirstLoop = false;
    bool WindowVisible = true;
    bool done = false;
   
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_SYSWMEVENT)
			{
				if (event.syswm.msg->msg.win.msg == winmsg)
				{
					if (LOWORD(event.syswm.msg->msg.win.lParam) == WM_LBUTTONUP)
					{
						TrayIcon(window, false);
						SDL_ShowWindow(window);
						WindowVisible = true;
					}
				}
			}
            if (event.type == SDL_QUIT)
            {
				TrayIcon(window, true);
                //done = true;
                SDL_HideWindow(window);
                WindowVisible = false;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            {
				TrayIcon(window, true);
                //done = true;
                SDL_HideWindow(window);
                WindowVisible = false;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(window))
            {
                float SizeX = static_cast<float>(event.window.data1);
                float SizeY = static_cast<float>(event.window.data2);

                ImVec2 MinSize = ImGui_Layout::MinimumDisplaySize();
                bool bResizeX = (SizeX < MinSize.x), bResizeY = (SizeY < MinSize.y);
                if (bResizeX || bResizeY)
                {
                    SDL_SetWindowSize(window,
                        (bResizeX ? static_cast<int>(MinSize.x) : static_cast<int>(SizeX)),
                        (bResizeY ? static_cast<int>(MinSize.y) : static_cast<int>(SizeY))
                    );
                }
            }
        }

        if (WindowVisible)
        {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            ImGui_Layout::BeginLayout();

            ImGui::ShowDemoWindow();

            /*
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)


            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }
            */

            //MainUI();

            /*
            ImGui_Layout::NextWindowWidth(325);
            ImGui_Layout::BeginAnchorWindow("Stats", { 0.f, 0.f }, { 0.0f, 1.f });
            {
                ImGui_Props::BeginPropertyGrid("TestGrid", false, true);

                ImGui_Props::PropertyGridTextInput("Name", Buf);

                static int CurItem = 0;
                const char* Types[] = { "Pistol", "Basic", "Heavy" };
                if (ImGui_Props::PropertyGridCombo("Type", &CurItem, Types, IM_ARRAYSIZE(Types)))
                {
                    bool b = true;
                }

                static int ClassItem = 8;
                const char* Classes[] = { "Bolt", "Exotic", "Flame", "Las", "Launcher", "Low-Tech", "Melta", "Plasma", "Solid Projectile" };
                ImGui_Props::PropertyGridCombo("Class", &ClassItem, Classes, IM_ARRAYSIZE(Classes));

                static float Weight = 10.f;
                static bool Show = true;
                static uint8_t Places = 0;
                ImGui_Props::PropertyGridFloatInput("Weight", Weight, &Show, &Places, 0.f, 1000.f, nullptr, "kg");

                ImGui_Props::PropertyGridTextInput("Damage", DmgBuf);
                
                static char RoF[64] = "S/-/-";
                ImGui_Props::PropertyGridTextInput("Rate of Fire", RoF);

                if (ImGui_Props::BeginCollapsingRegion("Rate of Fire"))
                {

                }
                ImGui_Props::EndCollapsingRegion();

                ImGui_Props::EndPropertyGrid();
            }
            ImGui_Layout::EndAnchorWindow();
            
            ImGui_Layout::NextWindowHorizontalAlign();

            ImGui_Layout::NextWindowWidthBounds(-1, 450);
            ImGui_Layout::BeginAnchorWindow("Preview", { 0.f, 0.f }, { 1.f, 1.f });
                // ...
            ImGui_Layout::EndAnchorWindow();
            */

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui_Layout::BeginAnchorWindow("Sus", { 0.f, 0.f }, { 1.f, 1.f }, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
                ImGui::BeginMenuBar();
                    if (ImGui::BeginMenu("File"))
                    {
                        ImGui::EndMenu();
                    }

                    static const char* Systems[] = { "Dark Heresy 1e", "Dark Heresy 2e" };
                    static size_t SelectedSystem = 1u;

                    if (ImGui::BeginMenu("System"))
                    {
                        for (size_t i = 0; i < IM_ARRAYSIZE(Systems); i++)
                        {
                            if (ImGui::MenuItem(Systems[i], nullptr, SelectedSystem == i))
                            {
                                SelectedSystem = i;
                            }
                        }
                        ImGui::EndMenu();
                    }
                ImGui::EndMenuBar();

                ImGui::BeginTabBar("Tabs");
                    if (ImGui::BeginTabItem("Quick Reference"))
                    {
                        ImGui::EndTabItem();
                    }
                    
                    if (ImGui::BeginTabItem("Weapons"))
                    {
                        ImGui::EndTabItem();
                    }
                    
                ImGui::EndTabBar();
            ImGui_Layout::EndAnchorWindow();
            ImGui::PopStyleVar();

            // Rendering
            ImGui::Render();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}