#include "StaticMesh.hlsli"
#include "PhongSpecular.hlsli"
#include "LambertDiffuse.hlsli"
#include "Helper.hlsli"

SamplerState smpPoint : register(s0);

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);


float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = albedoTex.Sample(smpPoint, pin.texcoord);
    float3 texNormal = normalTex.Sample(smpPoint, pin.texcoord);
    float3 N = GetNormal(pin.worldNormal, texNormal, pin.worldTangent);

    const float3 L = normalize(directionalLight.direction);

    const float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));

    const float3 specular = CalcPhongSpecular(N, L, directionalLight.color, E, shininess, ks);

    const float3 diffuse = CalcDiffuseColor(N, L, directionalLight.color, kd, color.rgb);

    //ポイントライトの処理

    //サーフェイスに入射するポイントライトのベクトル
    float3 ligDir = normalize(pin.worldPosition - pointLight.position);
    //減衰なしランバートディフューズを求める
    float3 diffusePoint = CalcLambertDiffuse(N, ligDir, pointLight.color, kd);
    //減衰なしフォンスペキュラを求める
    float3 specularPoint = CalcPhongSpecular(N, L, pointLight.color, E, shininess, float3(1, 1, 1));

    //距離から影響率を求める　距離に応じて減衰する
    float distance = length(pin.worldPosition - pointLight.position);
    //float affect = max(0, 1.0f - 1.0f / 1000 * distance);
    float affect = max(0, 1.0f - 1.0f / pointLight.range * distance);
    //累乗すればいい感じになるらしい
    affect = pow(affect, 1.0f);
    //affect = 0.1f;
    diffusePoint *= affect;
    specularPoint *= affect;
    float4 ret = float4(diffuse + specular + diffusePoint + specularPoint, 1);
    ret = float4(diffusePoint + specularPoint, 1);
    return ret;
}