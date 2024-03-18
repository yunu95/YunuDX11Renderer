#include "yd11CommonBuffer.hlsli"

struct VertexInput
{
    // 메시 버텍스는 로컬 포지션만 필요함.
    float3 position : POSITION;
};

struct PixelInput
{
    float4 posH : SV_POSITION; // Screen space position
    float3 posL : POSITION; // World space position
};

PixelInput main(VertexInput input)
{
    PixelInput output;
    output.posH = mul(float4(input.position, 0), cam_viewProjection).xyww;
    output.posL = input.position;
    //output.posL = mul(float4(input.position, 0), cam_world);
    return output;
}
