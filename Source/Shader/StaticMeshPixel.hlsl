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

    //�|�C���g���C�g�̏���
    float3 diffusePoint = 0;
    float3 specularPoint = 0;

    for (int i = 0; i < NumPointLight; ++i)
    {
        float3 pLColor = pointLight[i].color.rgb * pointLight[i].color.a;
		//�T�[�t�F�C�X�ɓ��˂���|�C���g���C�g�̃x�N�g��
        float3 ligDir = normalize(pin.worldPosition - pointLight[i].position);
		//�����Ȃ������o�[�g�f�B�t���[�Y�����߂�
        float3 dPoint = CalcDiffuseColor(N, ligDir, pLColor, kd, color.rgb * pin.color);
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
    float4 ret = float4(diffuse + specular + diffusePoint + specularPoint, materialColor.a);

    float3 emissive = emissiveColor.rgb * emissiveColor.a * color.rgb;
   //float4 ret = float4(diffuse + specular, 1);
	//float4 ret = float4(diffusePoint + specularPoint, 1);
    return float4(ret.rgb + emissive, ret.a);
}