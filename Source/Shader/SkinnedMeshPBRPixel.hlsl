#include "SkinnedMesh.hlsli"
#include "../DemoShaderMacro.h"
#include "LambertDiffuse.hlsli"
#include "PhongSpecular.hlsli"


Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D metallicTex : register(t2);
Texture2D roughnessTex : register(t3);

SamplerState smpPoint : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = albedoTex.Sample(smpPoint, pin.texcoord);
    float3 N = normalize(pin.worldNormal.xyz);

    float3 T = normalize(pin.worldTangent.xyz);
    float sigma = pin.worldTangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = normalTex.Sample(smpPoint, pin.texcoord);
    normal = (normal * 2.0f) - 1.0f;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));

    float3 L0 = normalize(float3(-light[0].direction.xyz));
    float3 L1 = normalize(float3(-light[1].direction.xyz));
    float3 L2 = normalize(float3(-light[2].direction.xyz));
   // float3 L3 = normalize(float3(-light[3].position.xyz));
    float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));


    
    float3 specular0 = CalcPhongSpecular(N, L0, light[0].color, E, shininess, ks);
    float3 specular1 = CalcPhongSpecular(N, L1, light[1].color, E, shininess, ks);
    float3 specular2 = CalcPhongSpecular(N, L2, light[2].color, E, shininess, ks);
 //   float3 specular3 = CalcPhongSpecular(N, L3, light[3].color, E, shininess, ks);


    float3 diffuseColor0 = CalcDiffuseColor(N, L0, light[0].color, kd, color.rgb);
    float3 diffuseColor1 = CalcDiffuseColor(N, L1, light[1].color, kd, color.rgb);
    float3 diffuseColor2 = CalcDiffuseColor(N, L2, light[2].color, kd, color.rgb);
 //   float3 diffuseColor3 = CalcDiffuseColor(N, L3, light[3].color, kd, color.rgb);


    float r = max(0, max(diffuseColor0.r, max(diffuseColor1.r, diffuseColor2.r)));
    float g = max(0, max(diffuseColor0.g, max(diffuseColor1.g, diffuseColor2.g)));
    float b = max(0, max(diffuseColor0.b, max(diffuseColor1.b, diffuseColor2.b)));
    //float r = clamp(diffuseColor0.r + diffuseColor1.r + diffuseColor2.r/* + diffuseColor3.r*/, 0, 1);
    //float g = clamp(diffuseColor0.g + diffuseColor1.g + diffuseColor2.g/* + diffuseColor3.g*/, 0, 1);
    //float b = clamp(diffuseColor0.b + diffuseColor1.b + diffuseColor2.b/* + diffuseColor3.b*/, 0, 1);

    float sr = max(0, max(specular0.r, max(specular1.r, specular2.r)));
    float sg = max(0, max(specular0.g, max(specular1.g, specular2.g)));
    float sb = max(0, max(specular0.b, max(specular1.b, specular2.b)));
    //float sr = clamp(specular0.r + specular1.r + specular2.r/* + specular3.r*/, 0, 1);
    //float sg = clamp(specular0.g + specular1.g + specular2.g/* + specular3.g*/, 0, 1);
    //float sb = clamp(specular0.b + specular1.b + specular2.b/* + specular3.b*/, 0, 1);
    float4 ret = float4(float3(r, g, b) + float3(sr, sg, sb), 1);

    return ret;
}
