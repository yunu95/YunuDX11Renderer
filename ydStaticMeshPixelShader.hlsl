//Texture2D<float4> positionTexture : register(t0);
//Texture2D<float4> normalTexture : register(t1);
//Texture2D<float4> albedoTexture : register(t2);
//Texture2D<float> metallicTexture : register(t3);
//Texture2D<float> roughnessTexture : register(t4);
//Texture2D<float4> ambientOcclusionTexture : register(t5);

struct PixelInput
{
    float4 position : SV_POSITION; // Screen space position
    float3 worldPosition : TEXCOORD0; // World space position
    float3 normal : TEXCOORD1; // Interpolated normal
    float2 texCoord : TEXCOORD2; // Interpolated texture coordinates
    float metallic : TEXCOORD3; // Interpolated metallic attribute
    float roughness : TEXCOORD4; // Interpolated roughness attribute
};
struct PixelOutput
{
    float4 position : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 albedo : SV_TARGET2;
    float4 metallic : SV_TARGET3;
    float4 roughness : SV_TARGET4;
    float4 ambientOcclusion : SV_TARGET5;
};
PixelOutput main(PixelInput input)
{
    PixelOutput output;
    output.position = float4(input.worldPosition, 1);
    output.normal = float4(input.normal, 1);
    output.albedo = float4(input.texCoord, 0, 1);
    output.metallic = float4(input.metallic, 0, 0, 1);
    output.roughness = float4(input.roughness, 0, 0, 1);
    output.ambientOcclusion = float4(1, 1, 1, 1);
    return output;
}