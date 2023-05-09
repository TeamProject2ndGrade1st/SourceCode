#include "SkinnedMesh.hlsli"
#include "../DemoShaderMacro.h"
#include "LambertDiffuse.hlsli"
#include "PhongSpecular.hlsli"
#include "Helper.hlsli"

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
SamplerState smpPoint : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = albedoTex.Sample(smpPoint, pin.texcoord);
    float3 normal = normalTex.Sample(smpPoint, pin.texcoord);
   
    float3 N = GetNormal(pin.worldNormal, normal, pin.worldTangent);

    float3 L = normalize(float3(directionalLight.direction.xyz));
  
    float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));


    
    float3 specular = CalcPhongSpecular(N, L, directionalLight.color, E, shininess, ks);
 

    float3 diffuse = CalcDiffuseColor(N, L, directionalLight.color, kd, color.rgb);

    float4 ret = float4(diffuse + specular, 1);
    return ret;
}