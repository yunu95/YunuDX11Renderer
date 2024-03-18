struct VS_INPUT
{
    float2 position : POSITION; // Screen space coordinates
    float2 uv : TEXCOORD;
    int textureIndex : TEXINDEX; // Texture register index
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    int textureIndex : TEXINDEX;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.position = float4(input.position, 0.0f, 1.0f); // Set z to 0 for screen space quad
    output.uv = input.uv;
    output.textureIndex = input.textureIndex;

    return output;
}