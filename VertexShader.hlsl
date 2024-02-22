 struct VertexInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};
struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};
PixelInput main(VertexInput input)
{
    PixelInput output;
    output.color = input.color;
    output.position = float4(input.position, 1);
    return output;
}