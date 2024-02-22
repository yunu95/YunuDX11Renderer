#pragma once
#include <vector>
#include <fstream>
#include "Singleton.h"
#include "DX11Dependency.h"
#include <unordered_set>
#include <unordered_map>

namespace dx11
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
	private:
		std::unordered_map<ID3D11VertexShader*, Microsoft::WRL::ComPtr<ID3D11VertexShader>> vertexShaders;
		std::unordered_map<ID3D11PixelShader*, Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelShaders;
		std::unordered_map<ID3D11VertexShader*, Microsoft::WRL::ComPtr<ID3D11InputLayout>> vsInputLayouts;
		std::unordered_map<ID3D11PixelShader*, Microsoft::WRL::ComPtr<ID3D11InputLayout>> psInputLayouts;
		std::unordered_map<std::string, ID3D11VertexShader*> vertexShadersByPath;
		std::unordered_map<std::string, ID3D11PixelShader*> pixelShadersByPath;

		ID3D11VertexShader* LoadVertexShaderFromFile(const char* filename);
		ID3D11PixelShader* LoadPixelShaderFromFile(const char* filename);
		std::vector<char> LoadFile(const char* filename);
	};
}