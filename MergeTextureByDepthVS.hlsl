// 렌더타겟으로 쓰인 두 텍스처들을 깊이값을 비교해가며 하나로 합치는 셰이더 코드
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = input.Pos;
    output.Tex = input.Tex;
    return output;
}