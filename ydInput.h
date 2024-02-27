#pragma once
#include <Windows.h>

class Input
{
public:
    template<int VKCode>
    static bool IsKeyDown()
    {
        return GetAsyncKeyState(VKCode) & 0x8000;
    }
    static void GetMousePosition(int& x, int& y) 
    {
        POINT currentMousePos;
        GetCursorPos(&currentMousePos);
        x = currentMousePos.x;
        y = currentMousePos.y;
    }
};
