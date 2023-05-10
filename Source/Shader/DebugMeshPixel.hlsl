#include "DebugMesh.hlsli"
#include "SceneConstant.hlsli"

float3 CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
    float d = max(0, dot(normal, -lightVector));

    return kd * lightColor * d;
}

float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor,
	float3 eyeVector, float shininess, float3 ks)
{
    float3 R = reflect(lightVector, normal);

    float d = max(dot(-eyeVector, R), 0);
    d = pow(d, shininess);

    return d * lightColor * ks;
}

float4 main(VSOut pin) : SV_TARGET
{
    return float4(1, 0, 0, 1);
}