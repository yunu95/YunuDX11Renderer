#include "yd11GBuffers.hlsli"
#include "yd11UVScreenPrimitives.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 tempDirLightAngle = float4(0.56, 0.56, 0.56, 1);
    float4 tempDirLightColor = float4(1, 1, 1, 1);
    float4 color = albedoTexture.Sample(gBufferSampler, input.uv);
    float3 normal = normalTexture.Sample(gBufferSampler, input.uv).xyz;
    float diffuseStrength = dot(normal, tempDirLightAngle);
    color = mul(tempDirLightColor, color) * diffuseStrength;

    return color;
}