#pragma once
#include "yd11DXDependency.h"
#include "yd11Transform.h"

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
