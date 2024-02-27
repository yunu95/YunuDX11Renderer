#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "ydSingleton.h"
#include "ydDX11Dependency.h"
#include <unordered_set>
#include <unordered_map>

namespace dx11
{
    class ResourceManager : public Singleton<ResourceManager>
    {
    public:
        ID3D11VertexShader* LoadVertexShaderFromFile(const WCHAR* filename);
        ID3D11PixelShader* LoadPixelShaderFromFile(const WCHAR* filename);
        ID3D11InputLayout* GetInputLayout(ID3D11VertexShader* shader);
        //ID3D11PixelShader* LoadPixelShaderFromFile(const char* filename);
    private:
        std::unordered_map<ID3D11VertexShader*, Microsoft::WRL::ComPtr<ID3D11VertexShader>> vertexShaders;
        std::unordered_map<ID3D11PixelShader*, Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelShaders;
        std::unordered_map<ID3D11VertexShader*, Microsoft::WRL::ComPtr<ID3D11InputLayout>> inputLayouts;
        std::unordered_map<std::wstring, ID3D11VertexShader*> vertexShadersByPath;
        std::unordered_map<std::wstring, ID3D11PixelShader*> pixelShadersByPath;

        //std::vector<char> LoadFile(const char* filename);
    };
}