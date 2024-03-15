#include "gBuffers.hlsli"
#include "UVScreenPrimitives.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = albedoTexture.Sample(gBufferSampler, input.uv);
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
    // 감마 코렉숀
    //color = color / (color + float4(1.0, 1.0, 1.0, 1.0));
    //color = pow(color, float4(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2, 1.0));
    //color.a = 1.0;
    return color;
}