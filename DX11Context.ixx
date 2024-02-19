module;
#include <Windows.h>
#include <d3d11.h>
export module DX11Context;

export namespace dx11
{
    class Context
    {
    public:
        // default window size
        static constexpr int Window_Width = 800;
        static constexpr int Height = 800;

        LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        int Init()
        {
            WNDCLASSEX wc = { 0 };
        };
    }
}
