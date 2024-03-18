#pragma once
#include "yd11DXDependency.h"

namespace dx11
{
    class Texture;
    class Material
    {
    public:
        Material(ID3D11VertexShader* vertexShader = nullptr,
            ID3D11PixelShader* pixelShader = nullptr,
            ID3D11ShaderResourceView* SRV_Albedo = nullptr,
            ID3D11ShaderResourceView* SRV_Normal = nullptr,
            ID3D11ShaderResourceView* SRV_ARM = nullptr);
        void SetAlbedoSRV(ID3D11ShaderResourceView* srv);
        void SetNormalSRV(ID3D11ShaderResourceView* srv);
        void SetARM_SRV(ID3D11ShaderResourceView* srv);
        ID3D11ShaderResourceView* GetAlbedoSRV() const;
        ID3D11ShaderResourceView* GetNormalSRV() const;
        ID3D11ShaderResourceView* GetARM_SRV() const;
        const std::array<ID3D11ShaderResourceView*, 3>& GetMaterialSRVs();

        ComPtr<ID3D11VertexShader> vertexShader;
        ComPtr<ID3D11PixelShader> pixelShader;
    private:
        ComPtr<ID3D11ShaderResourceView> SRV_Albedo;
        ComPtr<ID3D11ShaderResourceView> SRV_Normal;
        ComPtr<ID3D11ShaderResourceView> SRV_ARM;
        std::array<ID3D11ShaderResourceView*, 3> materialSRVs{nullptr};
    };
}
