#include "SkinnedMesh.hlsli"
#include "../DemoShaderMacro.h"
#include "LambertDiffuse.hlsli"
#include "PhongSpecular.hlsli"


Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
SamplerState smpPoint : register(s0);
//SamplerState smpAniso : register(s1);



float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = albedoTex.Sample(smpPoint, pin.texcoord);


    float3 normal = normalTex.Sample(smpPoint, pin.texcoord).rgb;

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

    float3 color1 = color.rgb * diffuse1.rgb;
    float3 color2 = color.rgb * diffuse2.rgb;
    float r = clamp(color1.r + color2.r, 0, 1);
    float g = clamp(color1.g + color2.g, 0, 1);
    float b = clamp(color1.b + color2.b, 0, 1);

    float sr = clamp(specular1.r + specular2.r, 0, 1);
    float sg = clamp(specular1.g + specular2.g, 0, 1);
    float sb = clamp(specular1.b + specular2.b, 0, 1);
    float4 ret = float4(float3(r, g, b) + float3(sr, sg, sb), 1);

    return ret;



	//float4 color = albedoTex.Sample(smpPoint, pin.texcoord);
 
	//float3 normal = normalTex.Sample(smpPoint, pin.texcoord).rgb;

	//float3 T = float3(1.0001, 0, 0);
	//float3x3 CM = {normalize(T), normalize(cross(pin.worldNormal.xyz, T)), normalize(pin.worldNormal.xyz) };


	////float3 N = normalize(pin.worldNormal.xyz);
	//float3 N = normalTex.Sample(smpPoint, pin.texcoord).rgb;
	//N = normalize(mul(normal * 2.0f - 1.0f, CM));

 //   float3 L1 = normalize(float3(-light[0].position.xyz));
 //   float3 L2 = normalize(float3(-light[1].position.xyz));
 //   float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));


 //   float3 diffuse1 = CalcLambertDiffuse(N, L1, light[0].color, kd);
 //   float3 diffuse2 = CalcLambertDiffuse(N, L2, light[1].color, kd);
 //   float3 specular1 = CalcPhongSpecular(N, L1, light[0].color, E, shininess, ks);
 //   float3 specular2 = CalcPhongSpecular(N, L2, light[1].color, E, shininess, ks);

 //   float3 color1 = color.rgb * diffuse1.rgb * pin.color.rgb;
 //   float3 color2 = color.rgb * diffuse2.rgb * pin.color.rgb;
 //   color1 = float3(clamp(color1.x, 0, 1), clamp(color1.y, 0, 1), clamp(color1.z, 0, 1));
 //   color2 = float3(clamp(color2.x, 0, 1), clamp(color2.y, 0, 1), clamp(color2.z, 0, 1));
 //   float r = clamp(color1.r + color2.r, 0, 1);
 //   float g = clamp(color1.g + color2.g, 0, 1);
 //   float b = clamp(color1.b + color2.b, 0, 1);

 //   float sr = clamp(specular1.r + specular2.r, 0, 1);
 //   float sg = clamp(specular1.g + specular2.g, 0, 1);
 //   float sb = clamp(specular1.b + specular2.b, 0, 1);
 //   float4 ret = float4(float3(r, g, b) + float3(sr, sg, sb), 1);

 //   return ret;
}