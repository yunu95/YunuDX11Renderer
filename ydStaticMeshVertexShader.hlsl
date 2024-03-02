#include "CommonBuffer.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float metallic : METALLIC;
    float roughness : ROUGHNESS;
};

struct PixelInput
{
    float4 position : SV_POSITION; // Screen space position
    float3 worldPosition : TEXCOORD0; // World space position
    float3 normal : TEXCOORD1; // Interpolated normal
    float2 texCoord : TEXCOORD2; // Interpolated texture coordinates
    float metallic : TEXCOORD3; // Interpolated metallic attribute
    float roughness : TEXCOORD4; // Interpolated roughness attribute};
};
PixelInput main(VertexInput input)
{
    PixelInput output;
    output.position = mul(float4(input.position, 1), viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.worldPosition = input.position;
    output.normal = input.normal;
    output.texCoord = input.texCoord;
    output.metallic = input.metallic;
    output.roughness = input.roughness;
    return output;
}