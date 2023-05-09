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

    float3 diffuse = CalcDiffuseColor(N, L, directionalLight.color, kd, color.rgb * materialColor);

    //ポイントライトの処理
    float3 diffusePoint = 0;
    float3 specularPoint = 0;

    for (int i = 0; i < NumPointLight; ++i)
    {
    //サーフェイスに入射するポイントライトのベクトル
        float3 ligDir = normalize(pin.worldPosition - pointLight[i].position);
    //減衰なしランバートディフューズを求める
        float3 dPoint = CalcDiffuseColor(N, ligDir, pointLight[i].color, kd, color.rgb * materialColor);
    //減衰なしフォンスペキュラを求める
        float3 sPoint = CalcPhongSpecular(N, L, pointLight[i].color, E, shininess, ks);

    //距離から影響率を求める　距離に応じて減衰する
        float distance = length(pin.worldPosition - pointLight[i].position);
    //float affect = max(0, 1.0f - 1.0f / 1000 * distance);
        //float affect = 0.8f;
        float affect = max(0, 1.0f - 1.0f / pointLight[i].range * distance);
    //累乗すればいい感じになるらしい
        affect = pow(affect, 2.0f);
    //affect = 0.1f;
        dPoint *= affect;
        sPoint *= affect;
        diffusePoint += dPoint;
        specularPoint += sPoint;
    }
    //float4 ret = float4(diffuse + specular + diffusePoint + specularPoint, 1);

   //float4 ret = float4(diffuse + specular, 1);
    float4 ret = float4(diffusePoint + specularPoint, 1);


    //float4 ret = float4(diffuse + specular, 1);
    return ret;
}