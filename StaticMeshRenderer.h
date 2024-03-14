#pragma once
#include "ydDX11Dependency.h"
#include "ydTransform.h"

namespace dx11
{
    class StaticMesh;
    class Material;
    class StaticMeshRenderer : public Transform
    {
    public:
        StaticMeshRenderer();
        ~StaticMeshRenderer();

        void Render();
        shared_ptr<Material> material;
        shared_ptr<StaticMesh> staticMesh;
    };
}
