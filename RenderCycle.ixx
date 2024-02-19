module;
#include <Windows.h>
export module renderCycle;
import d11Context;
import imgui_module;

export class Cycle
{
public:
    static void InitCycle()
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
        secondsPerCount = 1.0f / (float)countsPerSec;
        imgui::InitImgui();
        dx11::Context::GetInstance().Init();
    }
    static bool UpdateCycle()
    {
        bool success = true;
        INT64 prevTime = currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        INT64 deltaTime = (currTime - prevTime);
        dt = (float)deltaTime * secondsPerCount;
        success &= imgui::UpdateImgui();
        success &= dx11::Context::GetInstance().Update();

        return success;
    }
    static bool CleanUpCycle()
    {
        bool success = true;
        success &= imgui::ImguiCleanUp();
        return success;
    }
private:
    static void InitWindow()
    {
        //WNDCLASS wc = {};
        //wc.lpfnWndProc = DefWindowProc;
        //wc.hInstance = GetModuleHandle(NULL);
        //wc.lpszClassName = L"RenderCycle";
        //RegisterClass(&wc);
        //HWND window = CreateWindowEx(0, wc.lpszClassName, "RenderCycle", 0, 0, 0, 0, 0, 0, 0, wc.hInstance, 0);
        //ShowWindow(window, SW_HIDE);
    }
    static INT64 currTime;
    static INT64 countsPerSec;
    static INT64 deltaTime;
    static float secondsPerCount;
    static float dt;
};
INT64 Cycle::currTime;
INT64 Cycle::countsPerSec;
INT64 Cycle::deltaTime;
float Cycle::secondsPerCount;
float Cycle::dt;
