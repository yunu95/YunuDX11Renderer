#include "ydD11Context.h"
#include "ydD11ResourceManager.h"

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

    /*std::vector<char> ResourceManager::LoadFile(const char* filename)
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
    }*/
}