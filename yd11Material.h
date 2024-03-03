#pragma once
#include "ydDX11Dependency.h"

namespace dx11
{
    class Texture;
    class Material
    {
    public:
        Material() = default;
        Texture* albedoMap;
        Texture* normalMap;
        Texture* ARMMap;
    };
}
