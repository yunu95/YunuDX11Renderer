#include "gBuffers.hlsli"

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    int textureIndex : TEXINDEX;
};
//Texture2D<float4> positionTexture : register(t0);
//Texture2D<float4> normalTexture : register(t1);
//Texture2D<uint4> albedoTexture : register(t2);
//Texture2D<uint> metallicTexture : register(t3);
//Texture2D<uint> roughnessTexture : register(t4);
//Texture2D<uint4> ambientOcclusionTexture : register(t5);
float4 main(VS_OUTPUT output) : SV_TARGET
{
    float4 color = float4(1, 1, 1, 1);
    float soleColor = 1;
    switch (output.textureIndex)
    {
        case 0:
            color = positionTexture.Sample(gBufferSampler, output.uv);
            break;
        case 1:
            //colorUint4 = normalTexture.Load(int3(output.uv, 0));
            color = normalTexture.Sample(gBufferSampler, output.uv);
            //color = float4(colorUint4.x, colorUint4.y, colorUint4.z, 1);
            break;
        case 2:
            //colorUint4 = albedoTexture.Load(int3(output.uv, 0));
            color = albedoTexture.Sample(gBufferSampler, output.uv);
            //color = float4(colorUint4.x, colorUint4.y, colorUint4.z, 1);
            break;
        case 3:
            //colorUint = metallicTexture.Load(int3(output.uv, 0));
            //colorUint = metallicTexture.Sample(gBufferSampler, output.uv);
            soleColor = metallicTexture.Sample(gBufferSampler, output.uv);
            color = float4(soleColor, soleColor, soleColor, 1);
            //color = float4(colorUint, colorUint, colorUint, 1);
            break;
        case 4:
            soleColor = roughnessTexture.Sample(gBufferSampler, output.uv);
            color = float4(soleColor, soleColor, soleColor, 1);
            //colorUint = roughnessTexture.Sample(gBufferSampler, output.uv);
            //color = float4(colorUint, colorUint, colorUint, 1);
            break;
        case 5:
            //colorUint4 = ambientOcclusionTexture.Load(int3(output.uv, 0));
            //color = float4(colorUint4.x, colorUint4.y, colorUint4.z, 1);
            color = ambientOcclusionTexture.Sample(gBufferSampler, output.uv);
            break;
    }
    return color;
}