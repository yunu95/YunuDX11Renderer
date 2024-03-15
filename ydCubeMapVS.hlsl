#include "CommonBuffer.hlsli"
#include "UVScreenPrimitives.hlsli"

PS_INPUT main(VS_INPUT input) 
{
    PS_INPUT output;
    output.position = float4(input.position, 0.999999, 1);
    output.uv = input.uv;
    return output;
}