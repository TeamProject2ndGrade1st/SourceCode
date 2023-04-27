#include "StaticMesh.hlsli"
#include "PhongSpecular.hlsli"
#include "LambertDiffuse.hlsli"

SamplerState smpPoint : register(s0);

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);


float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = albedoTex.Sample(smpPoint, pin.texcoord);

	float3 normal = normalTex.Sample(smpPoint, pin.texcoord).rgb;

	float3 T = float3(1.0001, 0, 0);
	float3x3 CM = {normalize(T), normalize(cross(pin.worldNormal.xyz, T)), normalize(pin.worldNormal.xyz) };


	//float3 N = normalize(pin.worldNormal.xyz);
	float3 N = normalTex.Sample(smpPoint, pin.texcoord).rgb;
	N = normalize(mul(normal * 2.0f - 1.0f, CM));

	float3 L1 = normalize(float3(-light[0].position.xyz));
	float3 L2 = normalize(float3(-light[1].position.xyz));
	float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));


	//ディフューズ
    float3 diffuseColor1 = CalcDiffuseColor(N, L1, light[0].color, kd, color.rgb);
    float3 diffuseColor2 = CalcDiffuseColor(N, L2, light[1].color, kd, color.rgb);
    float r = clamp(diffuseColor1.r + diffuseColor2.r, 0, 1);
    float g = clamp(diffuseColor1.g + diffuseColor2.g, 0, 1);
    float b = clamp(diffuseColor1.b + diffuseColor2.b, 0, 1);

	//スペキュラー
	float3 specular1 = CalcPhongSpecular(N, L1, light[0].color, E, shininess, ks);
    float3 specular2 = CalcPhongSpecular(N, L2, light[1].color, E, shininess, ks);

    float sr = clamp(specular1.r + specular2.r, 0, 1);
    float sg = clamp(specular1.g + specular2.g, 0, 1);
    float sb = clamp(specular1.b + specular2.b, 0, 1);


	float4 ret = float4(float3(r, g, b) + float3(sr, sg, sb), 1);

	return ret;
}