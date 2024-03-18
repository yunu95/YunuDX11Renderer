#pragma once
#include "yd11DXDependency.h"

namespace dx11::primitives
{
    struct CubeMapMeshVertex {
        float x, y, z;
        std::byte padding0[4];
    };
}