#pragma once
#include "ydStaticMeshVertex.h"
#include "ydDX11Dependency.h"
#include "yd11CubeMapVertex.h"

namespace dx11
{
    class CubeMap
    {
    public:
        // 파 플레인에 해당하는 부분에 큐브맵을 그린다.
        void RenderToBackBuffer();
    private:
        ComPtr<ID3D11ShaderResourceView> cubeTexture;
    };

}
