#pragma once
#include "ydSingleton.h"
class Cycle;

class YNTime : public Singleton<YNTime>
{
public:
    float GetDeltaTime() { return deltaTime; };
private:
    void SetDeltaTime(float dt)
    {
        deltaTime = dt;
    }
    float deltaTime{ 0 };
    friend Cycle;
};
