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

    //�|�C���g���C�g�̏���

    //�T�[�t�F�C�X�ɓ��˂���|�C���g���C�g�̃x�N�g��
    float3 ligDir = normalize(pin.worldPosition - pointLight.position);
    //�����Ȃ������o�[�g�f�B�t���[�Y�����߂�
    float3 diffusePoint = CalcLambertDiffuse(N, ligDir, pointLight.color, kd);
    //�����Ȃ��t�H���X�y�L���������߂�
    float3 specularPoint = CalcPhongSpecular(N, L, pointLight.color, E, shininess, float3(1, 1, 1));

    //��������e���������߂�@�����ɉ����Č�������
    float distance = length(pin.worldPosition - pointLight.position);
    //float affect = max(0, 1.0f - 1.0f / 1000 * distance);
    float affect = max(0, 1.0f - 1.0f / pointLight.range * distance);
    //�ݏ悷��΂��������ɂȂ�炵��
    affect = pow(affect, 1.0f);
    //affect = 0.1f;
    diffusePoint *= affect;
    specularPoint *= affect;
    float4 ret = float4(diffuse + specular + diffusePoint + specularPoint, 1);
    ret = float4(diffusePoint + specularPoint, 1);
    return ret;
}