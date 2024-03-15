#pragma once
#include "ydStaticMeshVertex.h"
#include "ydDX11Dependency.h"
#include "yd11CubeMapVertex.h"

namespace dx11
{
    class CubeMap
    {
    public:
        static constexpr UINT vertexStride{ sizeof(primitives::CubeMapVertex) };
        static constexpr UINT indexStride{ sizeof(unsigned int) };
        static constexpr unsigned int vertexCount{ 4 };
        static constexpr unsigned int indexCount{ 6 };
        //const std::vector<StaticMeshVertex>& GetVertexVec() { return vertexVec; };
        //const std::vector<unsigned int>& GetIndexVec() { return indexVec; };
        //const ComPtr<ID3D11Buffer>& GetVertexBuffer() { return vertexBuffer; };
        //const ComPtr<ID3D11Buffer>& GetIndexBuffer() { return indexBuffer; };
        /*ID3D11Buffer* const* GetAddressOfVertexBuffer() { return vertexBuffer.GetAddressOf(); };
        ID3D11Buffer* const* GetAddressOfIndexBuffer() { return indexBuffer.GetAddressOf(); };
        unsigned int GetVertexCount() { return vertexCount; };
        unsigned int GetIndexCount() { return indexCount; };*/
        void SetVertexVec(const std::vector<primitives::StaticMeshVertex>& vertexVec);
        void SetIndexVec(const std::vector<unsigned int>& indexVec);
        void SetAiMesh(aiMesh* aimesh);
        // far 플레인에 해당하는 부분에 백버퍼를 그린다.
        void RenderToBackBuffer();
        aiMesh* GetaiMesh();
    private:
        std::vector<unsigned int> indexVec;
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
        aiMesh* aimesh{ nullptr };

        unsigned int materialCount{ 0 };
    };

}
