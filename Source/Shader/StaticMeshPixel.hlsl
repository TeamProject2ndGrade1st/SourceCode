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
    float3 dLightColor = directionalLight.color.rgb * directionalLight.color.a;
    float3 texNormal = normalTex.Sample(smpPoint, pin.texcoord);
    float3 N = GetNormal(pin.worldNormal, texNormal, pin.worldTangent);

    const float3 L = normalize(directionalLight.direction);

    const float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));

    const float3 specular = CalcPhongSpecular(N, L, dLightColor, E, shininess, ks);

    const float3 diffuse = CalcDiffuseColor(N, L, dLightColor, kd, color.rgb * pin.color);

    //ポイントライトの処理
    float3 diffusePoint = 0;
    float3 specularPoint = 0;

    for (int i = 0; i < NumPointLight; ++i)
    {
        float3 pLColor = pointLight[i].color.rgb * pointLight[i].color.a;
		//サーフェイスに入射するポイントライトのベクトル
        float3 ligDir = normalize(pin.worldPosition - pointLight[i].position);
		//減衰なしランバートディフューズを求める
        float3 dPoint = CalcDiffuseColor(N, ligDir, pLColor, kd, color.rgb * pin.color);
		//減衰なしフォンスペキュラを求める
        float3 sPoint = CalcPhongSpecular(N, L, pLColor, E, shininess, ks);

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
    float4 ret = float4(diffuse + specular + diffusePoint + specularPoint, materialColor.a);

    float3 emissive = emissiveColor.rgb * emissiveColor.a * color.rgb;
   //float4 ret = float4(diffuse + specular, 1);
	//float4 ret = float4(diffusePoint + specularPoint, 1);
    return float4(ret.rgb + emissive, ret.a);
}