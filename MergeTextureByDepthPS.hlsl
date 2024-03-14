// Sampler for the first render target's texture and depth
Texture2D Texture0 : register(t0);
Texture2D Depth0 : register(t1);

// Sampler for the second render target's texture and depth
Texture2D Texture1 : register(t2);
Texture2D Depth1 : register(t3);

// The pixel shader
float4 PS(float2 TexCoord : TEXCOORD) : SV_Target
{
    // Sample the depth from both depth textures
    float depth0 = Depth0.Sample(SampleType, TexCoord).r;
    float depth1 = Depth1.Sample(SampleType, TexCoord).r;

    // Compare depths
    if (depth0 < depth1)
    {
        // If the first texture is closer, output its color
        return Texture0.Sample(SampleType, TexCoord);
    }
    else
    {
        // Otherwise, output the second texture's color
        return Texture1.Sample(SampleType, TexCoord);
    }
}