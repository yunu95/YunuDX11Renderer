#include "ydD11Context.h"
#include "ydD11ResourceManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace dx11
{
    using namespace Microsoft::WRL;
    ID3D11VertexShader* ResourceManager::LoadVertexShaderFromFile(const wchar_t* filename)
    {
        //assert(vertexShadersByPath.find(filename) == vertexShadersByPath.end() && "redundant load of resources");
        if (auto shaderFound = vertexShadersByPath.find(filename); shaderFound != vertexShadersByPath.end())
            return shaderFound->second;
        HRESULT hr;
        ID3D11VertexShader* vertexShader;
        // BLOB으로 바이너리 쉐이더 코드를 직빵으로 읽어들이고 리플렉션을 시도할 수 있음.
        ComPtr<ID3DBlob> blob;
        D3DReadFileToBlob(filename, blob.GetAddressOf());
        ComPtr<ID3D11ShaderReflection> shaderReflection;
        hr = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, &shaderReflection);
        assert(SUCCEEDED(hr));

        Context::Instance().device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
        D3D11_SHADER_DESC shaderDesc;
        shaderReflection->GetDesc(&shaderDesc);

        // Create input layout description based on shader input signature
        /* 보통 인풋 레이아웃 데스크립션은 이렇게 생겼다.
        D3D11_INPUT_ELEMENT_DESC layout[]
            {
                {"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
                {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
            };
        */
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
        for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            shaderReflection->GetInputParameterDesc(i, &paramDesc);

            // Fill in input layout description
            D3D11_INPUT_ELEMENT_DESC elementDesc
            {
                //paramDesc.ComponentType
                .SemanticName = paramDesc.SemanticName,
                .SemanticIndex = paramDesc.SemanticIndex,
                .Format = DXGI_FORMAT_UNKNOWN,
                .InputSlot = 0,
                .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
                .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            };
            switch (paramDesc.ComponentType)
            {
            case D3D_REGISTER_COMPONENT_UINT32: elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
            case D3D_REGISTER_COMPONENT_SINT32: elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
            case D3D_REGISTER_COMPONENT_FLOAT32: elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
            default: abort(); break; // 쉐이더 포맷 정보가 이상망측합니다.
            }
            inputLayoutDesc.push_back(elementDesc);
        }

        inputLayouts[vertexShader] = nullptr;
        Context::Instance().device->CreateInputLayout(inputLayoutDesc.data(), static_cast<UINT>(inputLayoutDesc.size()), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayouts[vertexShader]);
        vertexShaders[vertexShader] = ComPtr<ID3D11VertexShader>{ vertexShader };
        vertexShadersByPath[filename] = vertexShader;
        return vertexShader;
    }
    ID3D11PixelShader* ResourceManager::LoadPixelShaderFromFile(const WCHAR* filename)
    {
        //assert(pixelShadersByPath.find(filename) == pixelShadersByPath.end() && "redundant load of resources");
        if (auto shaderFound = pixelShadersByPath.find(filename); shaderFound != pixelShadersByPath.end())
            return shaderFound->second;
        ID3D11PixelShader* pixelShader;
        ComPtr<ID3DBlob> blob;
        D3DReadFileToBlob(filename, blob.GetAddressOf());
        Context::Instance().device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
        pixelShaders[pixelShader] = ComPtr<ID3D11PixelShader>{ pixelShader };
        pixelShadersByPath[filename] = pixelShader;
        return pixelShader;
    }
    ID3D11InputLayout* ResourceManager::GetInputLayout(ID3D11VertexShader* shader)
    {
        if (auto itr = inputLayouts.find(shader); itr != inputLayouts.end())
            return itr->second.Get();
        else
            return nullptr;
    }
    const aiScene* ResourceManager::LoadFBXFile(const char* fileName)
    {
        if (auto itr = aiScenesByPath.find(fileName); itr != aiScenesByPath.end())
            return itr->second;

        auto importer = std::make_unique<Assimp::Importer>();
        const aiScene* scene = importer->ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

        assert(!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) && "Failed to load FBX file");

        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            auto staticMesh = std::make_unique<StaticMesh>();
            // FBX에서 읽어들인 메쉬 정보를 쉐이더에서 쓰게될 메시 데이터로 변환
            std::vector<StaticMeshVertex> vertices = std::vector<StaticMeshVertex>(mesh->mNumVertices);
            std::vector<unsigned int> indices = std::vector<unsigned int>(mesh->mNumFaces * 3);
            for (unsigned int vi = 0; vi < mesh->mNumVertices; vi++)
            {
                vertices[vi].pos = { mesh->mVertices[vi].x, mesh->mVertices[vi].y, mesh->mVertices[vi].z };
                vertices[vi].normal = { mesh->mNormals[vi].x, mesh->mNormals[vi].y, mesh->mNormals[vi].z };
                if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
                {
                    vertices[vi].uv = { mesh->mTextureCoords[0][vi].x, mesh->mTextureCoords[0][vi].y };
                }
                else
                {
                    vertices[vi].uv = { 0.0f, 0.0f };
                }
            }
            for (unsigned int fi = 0; fi < mesh->mNumFaces; fi++)
            {
                assert(mesh->mFaces[fi].mNumIndices == 3);
                auto face = mesh->mFaces[fi];
                indices[fi * 3 + 0] = face.mIndices[0];
                indices[fi * 3 + 1] = face.mIndices[1];
                indices[fi * 3 + 2] = face.mIndices[2];
            }
            staticMesh->SetVertexVec(std::move(vertices));
            staticMesh->SetIndexVec(std::move(indices));
            staticMeshesByaiMesh[mesh] = staticMesh.get();
            staticMeshes[staticMesh.get()] = std::move(staticMesh);
        }
        aiScenes.insert(scene);
        aiScenesByPath[fileName] = scene;
        usedAiImporters.push_back(std::move(importer));
        return scene;
    }
    StaticMesh* ResourceManager::GetMesh(aiMesh* aimesh)
    {
        if (auto itr = staticMeshesByaiMesh.find(aimesh); itr != staticMeshesByaiMesh.end())
            return itr->second;
        return nullptr;
    }
}