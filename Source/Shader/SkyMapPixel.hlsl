#include "FullscreenQuad.hlsli"
#include "SceneConstant.hlsli"


SamplerState smp : register(s0);
Texture2D skybox : register(t0);

//todo ��قǗ������邱��
float4 sample_skybox(float3 v, float roughness)
{
    const float PI = 3.14159265358979;

    uint width, height, number_of_levels;
    skybox.GetDimensions(0, width, height, number_of_levels);

    //�����~�b�v�}�b�v�Ɖ����g���悤�H�@�����炭
    float lod = roughness * float(number_of_levels - 1);
    v = normalize(v);

	//z��x����^���W�F���g�����߂�����p�x�ɕϊ�����@���̏ꍇ�͔͈͂�
    //-pi ~ pi�ɂȂ�@uv���W��0~1�͈̔͂Ɏ��߂����̂ł��������̌v�Z�����g��
    //y�̏ꍇ�̓T�C������p�x�����߁A�^��������Ƃ���PI(���̂Ƃ���V���W�͂O�j
    //�^�����������Ƃ���-pI(���̂Ƃ���V���W��1)�ɂȂ�悤�Ȃ��������̌v�Z�����g��
    float2 sample_point;
    sample_point.x = (atan2(v.z, v.x) + PI) / (PI * 2.0);
    sample_point.y = 1.0 - ((asin(v.y) + PI * 0.5) / PI);
    
    return skybox.SampleLevel(smp, sample_point, lod);
}

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 ndc;
    //UV���W��p����NDC���W�n�ɕϊ����� �w�i�̏ꍇ�͈�ԉ��ɕ`�悳��邽��
    //Z�l�͂P
    ndc.x = (pin.texcoord.x * 2.0) - 1.0;
    ndc.y = 1.0 - (pin.texcoord.y * 2.0);
    ndc.z = 1;
    ndc.w = 1;

    //�r���[�v���W�F�N�V�����s��̋t�s��������邱�ƂŃ��[���h���W�֕ϊ�
    float4 R = mul(ndc, invViewProjection);

    //����͂Ȃ�ł����H�H�H
    R /= R.w;
    const float roughness = 0;
    return sample_skybox(R.xyz, roughness);
}
