#include "CommonBuffer.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv_albedo : TEXCOORD0;
    float2 uv_metallic : TEXCOORD1;
    float2 uv_normal : TEXCOORD2; 
    float2 uv_roughness : TEXCOORD3;
    float2 uv_ao : TEXCOORD4;
};

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
PixelInput main(VertexInput input)
{
    PixelInput output;
    output.position = mul(float4(input.position, 1), viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.worldPosition = mul(float4(input.position, 1), viewMatrix);
    output.normal = mul(float4(input.normal, 0), viewMatrix);
    output.uv_albedo = input.uv_albedo;
    output.uv_normal = input.uv_normal;
    output.uv_metallic = input.uv_metallic;
    output.uv_roughness = input.uv_roughness;
    output.uv_ao = input.uv_ao;
    return output;
}