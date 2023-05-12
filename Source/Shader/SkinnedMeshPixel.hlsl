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

    float3 dLightColor = directionalLight.color.a * directionalLight.color.rgb;
   
    float3 N = GetNormal(pin.worldNormal, normal, pin.worldTangent);

    float3 L = normalize(float3(directionalLight.direction.xyz));
  
    float3 E = normalize(float3(cameraPosition.xyz - pin.worldPosition.xyz));

    float3 specular = CalcPhongSpecular(N, L, dLightColor, E, shininess, ks);

    float3 diffuse = CalcDiffuseColor(N, L, dLightColor, kd, color.rgb * materialColor);

    //�|�C���g���C�g�̏���
    float3 diffusePoint = 0;
    float3 specularPoint = 0;

    for (int i = 0; i < NumPointLight; ++i)
    {
        float3 pLColor = pointLight[i].color.rgb * pointLight[i].color.a;
		//�T�[�t�F�C�X�ɓ��˂���|�C���g���C�g�̃x�N�g��
        float3 ligDir = normalize(pin.worldPosition - pointLight[i].position);
		//�����Ȃ������o�[�g�f�B�t���[�Y�����߂�
        float3 dPoint = CalcDiffuseColor(N, ligDir, pLColor, kd, color.rgb * materialColor);
		//�����Ȃ��t�H���X�y�L���������߂�
        float3 sPoint = CalcPhongSpecular(N, L, pLColor, E, shininess, ks);

		//��������e���������߂�@�����ɉ����Č�������
        float distance = length(pin.worldPosition - pointLight[i].position);
		//float affect = max(0, 1.0f - 1.0f / 1000 * distance);
        //float affect = 0.8f;
        float affect = max(0, 1.0f - 1.0f / pointLight[i].range * distance);
		//�ݏ悷��΂��������ɂȂ�炵��
        affect = pow(affect, 2.0f);
		//affect = 0.1f;
        dPoint *= affect;
        sPoint *= affect;
        diffusePoint += dPoint;
        specularPoint += sPoint;
    }
    float4 ret = float4(diffuse + specular + diffusePoint + specularPoint, 1);

    float3 emissive = emissiveColor.rgb * emissiveColor.a * color.rgb;
   //float4 ret = float4(diffuse + specular, 1);
    //float4 ret = float4(diffusePoint + specularPoint, 1);


    //float4 ret = float4(diffuse + specular, 1);
    return float4(ret.rgb + emissive, ret.a);
}