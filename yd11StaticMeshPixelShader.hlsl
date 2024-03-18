#include "yd11CommonBuffer.hlsli"
#include "yd11MaterialBuffers.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION; // Screen space position
    float3 worldPosition : TEXCOORD0; // World space position
    float3 normal : NORMAL; // Interpolated normal
    float2 uv_albedo : TEXCOORD1; // Interpolated texture coordinates
    float2 uv_normal : TEXCOORD2; // Interpolated texture coordinates
    float2 uv_metallic : TEXCOORD3; // Interpolated metallic attribute
    float2 uv_roughness : TEXCOORD4; // Interpolated roughness attribute
    float2 uv_ao : TEXCOORD5; // Interpolated roughness attribute
};
struct PixelOutput
{
    float4 position : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 albedo : SV_TARGET2;
    float4 arm : SV_TARGET3; // Metallic에다가 그냥 ARM 다 때려박는게 낫겟다.
    //float4 roughness : SV_TARGET4;
    //float4 ambientOcclusion : SV_TARGET5;
};
PixelOutput main(PixelInput input)
{
    PixelOutput output;
    output.position = float4(input.worldPosition, 1);
    output.normal = float4(input.normal, 1);
    output.albedo = albedoMap.Sample(materialSampler, input.uv_albedo);
    //output.normal = normalMap.Sample(materialSampler, input.uv_normal);
    //output.metallic = ;
    //output.roughness = ;
    output.arm = ARMMap.Sample(materialSampler, input.uv_metallic);
    //output.arm = float4(output.metallic.r, float4(output.metallic.g, 0, 0, 1), ARMMap.Sample(materialSampler, input.uv_metallic), 1);
    return output;
}