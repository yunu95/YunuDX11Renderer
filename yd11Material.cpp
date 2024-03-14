#pragma once
#include "yd11Material.h"

namespace dx11
{
    Material::Material(ID3D11VertexShader* vertexShader,
        ID3D11PixelShader* pixelShader,
        ID3D11ShaderResourceView* SRV_Albedo,
        ID3D11ShaderResourceView* SRV_Normal,
        ID3D11ShaderResourceView* SRV_ARM)
        : vertexShader(vertexShader),
        pixelShader(pixelShader),
        SRV_Albedo(SRV_Albedo),
        SRV_Normal(SRV_Normal),
        SRV_ARM(SRV_ARM)
    {
        ID3D11ShaderResourceView* materialSRVs[3] = { nullptr };
    }
    void Material::SetAlbedoSRV(ID3D11ShaderResourceView* srv)
    {
        materialSRVs[0] = srv;
        SRV_Albedo = srv;
    }
    void Material::SetNormalSRV(ID3D11ShaderResourceView* srv)
    {
        materialSRVs[1] = srv;
        SRV_Normal = srv;
    }
    void Material::SetARM_SRV(ID3D11ShaderResourceView* srv)
    {
        materialSRVs[2] = srv;
        SRV_ARM = srv;
    }
    ID3D11ShaderResourceView* Material::GetAlbedoSRV() const
    {
        return SRV_Albedo.Get();
    }
    ID3D11ShaderResourceView* Material::GetNormalSRV() const
    {
        return SRV_Normal.Get();
    }
    ID3D11ShaderResourceView* Material::GetARM_SRV() const
    {
        return SRV_ARM.Get();
    }
    const std::array<ID3D11ShaderResourceView*, 3>& Material::GetMaterialSRVs()
    {
        return materialSRVs;
    }
};
