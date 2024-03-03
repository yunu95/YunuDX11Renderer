Texture2D<float4> positionTexture : register(t0);
Texture2D<float4> normalTexture : register(t1);
Texture2D<float4> albedoTexture : register(t2);
Texture2D<float> ARMTexture : register(t3);
Texture2D<float> roughnessTexture : register(t4);
Texture2D<float4> ambientOcclusionTexture : register(t5);
SamplerState gBufferSampler : register(s0);
