#include "D11Context.h"
#include "D11ResourceManager.h"

namespace dx11
{
	using namespace Microsoft::WRL;
	ID3D11VertexShader* ResourceManager::LoadVertexShaderFromFile(const char* filename)
	{
		assert(vertexShadersByPath.find(filename) == vertexShadersByPath.end() && "redundant load of resources");
		if (auto shaderFound = vertexShadersByPath.find(filename); shaderFound != vertexShadersByPath.end())
			return shaderFound->second;
		ID3D11VertexShader* vertexShader;
		auto byteCode = LoadFile(filename);
		// BLOB으로 바이너리 쉐이더 코드를 직빵으로 읽어들이고 리플렉션을 시도할 수 있음.
		//ID3DBlob* blob = D3DReadFileToBlob(filename,&blob);
		//blob->
		//loadfrom
		Context::Instance().device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &vertexShader);
		vertexShaders[vertexShader] = ComPtr<ID3D11VertexShader>{ vertexShader };
		vertexShadersByPath[filename] = vertexShader;
		return vertexShader;
	}
	ID3D11PixelShader* ResourceManager::LoadPixelShaderFromFile(const char* filename)
	{
		assert(pixelShadersByPath.find(filename) == pixelShadersByPath.end() && "redundant load of resources");
		if (auto shaderFound = pixelShadersByPath.find(filename); shaderFound != pixelShadersByPath.end())
			return shaderFound->second;
		ID3D11PixelShader* pixelShader;
		auto byteCode = LoadFile(filename);
		Context::Instance().device->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &pixelShader);
		pixelShaders[pixelShader] = ComPtr<ID3D11PixelShader>{ pixelShader };
		pixelShadersByPath[filename] = pixelShader;
		return pixelShader;
	}
	std::vector<char> ResourceManager::LoadFile(const char* filename)
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			throw std::runtime_error("Failed miserably while opening shader file.");
		}
		size_t size = file.tellg();
		std::vector<char> bytecode(size);
		file.seekg(0);
		file.read(bytecode.data(), size);
		file.close();
		return bytecode;
	}
}