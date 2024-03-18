#include "yd11CommonBuffer.hlsli"
TextureCube cubeMap : register(t0);
SamplerState cubeMapSampler : register(s0);

struct PixelInput
{
    float4 posH : SV_POSITION; // Screen space position
    float3 posL : POSITION; // World space position
};

float4 main(PixelInput input) : SV_TARGET
{
    //float4 color = albedoTexture.Sample(gBufferSampler, input.uv);
    return cubeMap.Sample(cubeMapSampler, input.posL);
}