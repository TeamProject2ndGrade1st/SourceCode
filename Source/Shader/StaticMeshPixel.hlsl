#include "StaticMesh.hlsli"
#include "PhongSpecular.hlsli"
#include "LambertDiffuse.hlsli"

SamplerState smpPoint : register(s0);
//SamplerState smpAniso : register(s1);

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);

//float3 CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
//{
//	float d = max(0, dot(normal, -lightVector));

//	return kd * lightColor * d;
//}

//float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor,
//	float3 eyeVector, float shininess, float3 ks)
//{
//	float3 R = reflect(lightVector, normal);

//	float d = max(dot(-eyeVector, R), 0);
//	d = pow(d, shininess);

//	return d * lightColor * ks;
//}

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

	float3 L1 = normalize(float3(-light[0].position.xyz));
	float3 L2 = normalize(float3(-light[1].position.xyz));
	float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));


	float3 diffuse1 = CalcLambertDiffuse(N, L1, light[0].color, kd);
	float3 diffuse2 = CalcLambertDiffuse(N, L2, light[1].color, kd);
	float3 specular1 = CalcPhongSpecular(N, L1, light[0].color, E, shininess, ks);
    float3 specular2 = CalcPhongSpecular(N, L2, light[1].color, E, shininess, ks);

    float3 color1 = color.rgb * diffuse1.rgb * pin.color.rgb;
    float3 color2 = color.rgb * diffuse2.rgb * pin.color.rgb;
    float r = clamp(color1.r + color2.r, 0, 1);
    float g = clamp(color1.g + color2.g, 0, 1);
    float b = clamp(color1.b + color2.b, 0, 1);

    float sr = clamp(specular1.r + specular2.r, 0, 1);
    float sg = clamp(specular1.g + specular2.g, 0, 1);
    float sb = clamp(specular1.b + specular2.b, 0, 1);
	float4 ret = float4(float3(r, g, b) + float3(sr, sg, sb), 1);

	return ret;
}