/// <summary>
/// �t���l�����˂��l�������g�U���˂��v�Z
/// </summary>
/// <remark>
/// ���̊֐��̓t���l�����˂��l�������g�U���˗����v�Z���܂�
/// �t���l�����˂́A�������̂̕\�ʂŔ��˂��錻�ۂ̂Ƃ��ŁA���ʔ��˂̋����ɂȂ�܂�
/// ����g�U���˂́A�������̂̓����ɓ����āA�����������N�����āA�g�U���Ĕ��˂��Ă������̂��Ƃł�
/// �܂�t���l�����˂��ア�Ƃ��ɂ́A�g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��́A�g�U���˂��������Ȃ�܂�
///
/// </remark>
/// <param name="N">�@��</param>
/// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
/// <param name="V">�����Ɍ������x�N�g���B</param>
/// <param name="roughness">�e���B0�`1�͈̔́B</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-1 �f�B�Y�j�[�x�[�X�̃t���l�����˂ɂ��g�U���˂�^�ʖڂɎ�������B
    float3 H = normalize(L + V);
    const float roughness = 0.5f;
    const float energyBias = lerp(0.0f, 0.4f, roughness);
    const float energyFactor = lerp(1.0f, 1.0 / 1.51, roughness);

    const float dotLH = saturate(dot(L, H));
    const float Fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;
    const float dotNL = saturate(dot(N, L));
    const float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));
    const float dotNV = saturate(dot(N, V));
    const float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));
    return FL * FV * energyFactor;

}