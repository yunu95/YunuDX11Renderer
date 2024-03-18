#pragma once
#include "yd11StaticMeshVertex.h"
#include "yd11CubeMapMeshVertex.h"
#include "yd11DXDependency.h"
#include "yd11Texture.h"

namespace dx11
{
    class CubeMap
    {
    public:
        // 파 플레인에 해당하는 부분에 큐브맵을 그린다.
        CubeMap();
        void RenderToBackBuffer();
        void SetCubeMapMesh(aiMesh* meshInfo);
        void SetTexture(Texture* texture) { this->texture = texture; }
        static constexpr UINT vertexStride = sizeof(primitives::CubeMapMeshVertex);
    private:
        ComPtr<ID3D11Buffer> vertexBuffer;
        ComPtr<ID3D11Buffer> indexBuffer;
        ComPtr<ID3D11ShaderResourceView> cubeTexture;
        ComPtr<ID3D11RasterizerState> rasterizerState;
        Texture* texture;
        aiMesh* meshInfo;
        unsigned int indexCount;
    };

}
