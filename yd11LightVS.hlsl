#include "yd11UVScreenPrimitives.hlsli"
#include "yd11CommonBuffer.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.position = float4(input.position, 0, 1);
    output.uv = input.uv;
    return output;
}