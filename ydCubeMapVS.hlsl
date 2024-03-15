#include "CommonBuffer.hlsli"

struct VS_INPUT
{
    float2 position : POSITION; // Screen space coordinates
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

PS_INPUT main(VS_INPUT input) 
{
    PS_INPUT output;
    output.position = float4(input.position, 1, 1);
    output.uv = input.uv;
    return output;
}