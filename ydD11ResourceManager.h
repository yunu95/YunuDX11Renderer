#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "ydSingleton.h"
#include "ydDX11Dependency.h"
#include "yd11StaticMesh.h"
#include <unordered_set>
#include <unordered_map>

namespace dx11
{
    class StaticMesh;
    class ResourceManager : public Singleton<ResourceManager>
    {
    public:
        ID3D11VertexShader* LoadVertexShaderFromFile(const wchar_t* filename);
        ID3D11PixelShader* LoadPixelShaderFromFile(const WCHAR* filename);
        ID3D11InputLayout* GetInputLayout(ID3D11VertexShader* shader);
        const aiScene* LoadFBXFile(const char* fileName);
        StaticMesh* GetMesh(aiMesh* aimesh);
    private:
        std::unordered_map<ID3D11VertexShader*, Microsoft::WRL::ComPtr<ID3D11VertexShader>> vertexShaders;
        std::unordered_map<ID3D11PixelShader*, Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelShaders;
        std::unordered_map<ID3D11VertexShader*, Microsoft::WRL::ComPtr<ID3D11InputLayout>> inputLayouts;
        std::unordered_map<std::wstring, ID3D11VertexShader*> vertexShadersByPath;
        std::unordered_map<std::wstring, ID3D11PixelShader*> pixelShadersByPath;
        std::unordered_set<const aiScene*> aiScenes;
        std::vector<std::unique_ptr<Assimp::Importer>> usedAiImporters;
        std::unordered_map<std::string, const aiScene*> aiScenesByPath;
        std::unordered_map<StaticMesh*, std::unique_ptr<StaticMesh>> staticMeshes;
        std::unordered_map<aiMesh*, StaticMesh*> staticMeshesByaiMesh;
    };
}