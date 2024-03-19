#include "yd11GBuffers.hlsli"
#include "yd11UVScreenPrimitives.hlsli"
#include "yd11CommonBuffer.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 tempDirLightAngle = float3(0.56, 0.56, 0.56);
    float4 tempDirLightColor = float4(1, 0, 0, 1);
    float4 color = albedoTexture.Sample(gBufferSampler, input.uv);
    float3 normal = normalTexture.Sample(gBufferSampler, input.uv).xyz;
    //float4x4 tempView = float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    tempDirLightAngle = mul(float4(tempDirLightAngle, 0), cam_view).xyz;
    float diffuseStrength = dot(normal, tempDirLightAngle);
    //color = mul(tempDirLightColor, color) * diffuseStrength;
    color = tempDirLightColor * color * diffuseStrength;
    //color = float4(tempDirLightAngle, 1);
    //color = cam_view[0];
    //color = float4(normal.xyz, 1);

    return color;
}


//static const float3 f0 = float3(0.04, 0.04, 0.04);
//static const float MinRoughness = 0.04;
//static const float PI = 3.141592653589793;

//float3 getIBLContribution(float perceptualRoughness, float NdotV, float3 diffuseColor, float3 specularColor, float3 n, float3 reflection)
//{
//    const float lod = perceptualRoughness * NumSpecularMipLevels;

//    const float3 brdf = BRDFTexture.Sample(BRDFSampler, float2(NdotV, 1.0 - perceptualRoughness)).rgb;

//    const float3 diffuseLight = DiffuseTexture.Sample(IBLSampler, n).rgb;
//    const float3 specularLight = SpecularTexture.SampleLevel(IBLSampler, reflection, lod).rgb;

//    const float3 diffuse = diffuseLight * diffuseColor;
//    const float3 specular = specularLight * (specularColor * brdf.x + brdf.y);

//    return diffuse + specular;
//}

//float3 diffuse(float3 diffuseColor)
//{
//    return diffuseColor / PI;
//}

//float3 specularReflection(float3 reflectance0, float3 reflectance90, float VdotH)
//{
//    return reflectance0 + (reflectance90 - reflectance0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);
//}

//float geometricOcclusion(float NdotL, float NdotV, float alphaRoughness)
//{
//    const float attenuationL = 2.0 * NdotL / (NdotL + sqrt(alphaRoughness * alphaRoughness + (1.0 - alphaRoughness * alphaRoughness) * (NdotL * NdotL)));
//    const float attenuationV = 2.0 * NdotV / (NdotV + sqrt(alphaRoughness * alphaRoughness + (1.0 - alphaRoughness * alphaRoughness) * (NdotV * NdotV)));
//    return attenuationL * attenuationV;
//}

//float microfacetDistribution(float NdotH, float alphaRoughness)
//{
//    const float roughnessSq = alphaRoughness * alphaRoughness;
//    const float f = (NdotH * roughnessSq - NdotH) * NdotH + 1.0;
//    return roughnessSq / (PI * f * f);
//}

//float4 main(PSInputPbr input) : SV_TARGET
//{
//    const int idx = input.ViewId % 2;

//    // Roughness is stored in the 'g' channel, metallic is stored in the 'b' channel.
//    // This layout intentionally reserves the 'r' channel for (optional) occlusion map data
//    const float3 mrSample = MetallicRoughnessTexture.Sample(MetallicRoughnessSampler, input.TexCoord0);
//    const float4 baseColor = BaseColorTexture.Sample(BaseColorSampler, input.TexCoord0) * BaseColorFactor;
//    const float metallic = saturate(mrSample.b * MetallicFactor);
//    const float perceptualRoughness = clamp(mrSample.g * RoughnessFactor, MinRoughness, 1.0);

//    // Roughness is authored as perceptual roughness; as is convention,
//    // convert to material roughness by squaring the perceptual roughness [2].
//    const float alphaRoughness = perceptualRoughness * perceptualRoughness;

//    const float3 diffuseColor = (baseColor.rgb * (float3(1.0, 1.0, 1.0) - f0)) * (1.0 - metallic);
//    const float3 specularColor = lerp(f0, baseColor.rgb, metallic);

//    // Compute reflectance.
//    const float reflectance = max(max(specularColor.r, specularColor.g), specularColor.b);

//    // For typical incident reflectance range (between 4% to 100%) set the grazing reflectance to 100% for typical fresnel effect.
//    // For very low reflectance range on highly diffuse objects (below 4%), incrementally reduce grazing reflecance to 0%.
//    const float reflectance90 = saturate(reflectance * 25.0);
//    const float3 specularEnvironmentR0 = specularColor.rgb;
//    const float3 specularEnvironmentR90 = float3(1.0, 1.0, 1.0) * reflectance90;

//    // normal at surface point
//    float3 n = 2.0 * NormalTexture.Sample(NormalSampler, input.TexCoord0) - 1.0;
//    n = normalize(mul(n * float3(NormalScale, NormalScale, 1.0), input.TBN));

//    const float3 v = normalize(EyePosition[idx] - input.PositionWorld);   // Vector from surface point to camera
//    const float3 l = normalize(LightDirection);                           // Vector from surface point to light
//    const float3 h = normalize(l + v);                                    // Half vector between both l and v
//    const float3 reflection = -normalize(reflect(v, n));

//    const float NdotL = clamp(dot(n, l), 0.001, 1.0);
//    const float NdotV = abs(dot(n, v)) + 0.001;
//    const float NdotH = saturate(dot(n, h));
//    const float LdotH = saturate(dot(l, h));
//    const float VdotH = saturate(dot(v, h));

//    // Calculate the shading terms for the microfacet specular shading model
//    const float3 F = specularReflection(specularEnvironmentR0, specularEnvironmentR90, VdotH);
//    const float G = geometricOcclusion(NdotL, NdotV, alphaRoughness);
//    const float D = microfacetDistribution(NdotH, alphaRoughness);

//    // Calculation of analytical lighting contribution
//    const float3 diffuseContrib = (1.0 - F) * diffuse(diffuseColor);
//    const float3 specContrib = F * G * D / (4.0 * NdotL * NdotV);
//    float3 color = NdotL * LightColor * (diffuseContrib + specContrib);

//    // Calculate lighting contribution from image based lighting source (IBL)
//    color += getIBLContribution(perceptualRoughness, NdotV, diffuseColor, specularColor, n, reflection);

//    // Apply optional PBR terms for additional (optional) shading
//    const float ao = OcclusionTexture.Sample(OcclusionSampler, input.TexCoord0).r;
//    color = lerp(color, color * ao, OcclusionStrength);

//    const float3 emissive = EmissiveTexture.Sample(EmissiveSampler, input.TexCoord0) * EmissiveFactor;
//    color += emissive;

//    return float4(color, baseColor.a);
//}