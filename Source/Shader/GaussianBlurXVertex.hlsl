#include "GaussianBlur.hlsli"

VS_OUT main(in uint vertexId : SV_VERTEXID)
{
    VS_OUT Out;

    // ���W�ϊ�
    const float2 position[4] = { { -1, +1 }, { +1, +1 }, { -1, -1 }, { +1, -1 } };
    
    const float2 texcoords[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    
    Out.position = float4(position[vertexId], 0, 1);


    // �e�N�X�`���T�C�Y���擾
    float2 texSize;
    float level;
    tex.GetDimensions(0, texSize.x, texSize.y, level);

    // ��e�N�Z����UV���W���L�^
    float2 tex = texcoords[vertexId];

    // ��e�N�Z������U���W��+1�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    Out.tex0.xy = float2(1.0f / texSize.x, 0.0f);

    // ��e�N�Z������U���W��+3�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����

    Out.tex1.xy = float2(3.0f / texSize.x, 0.0f);

    // ��e�N�Z������U���W��+5�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    Out.tex2.xy = float2(5.0f / texSize.x, 0.0f);

    // ��e�N�Z������U���W��+7�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    Out.tex3.xy = float2(7.0f / texSize.x, 0.0f);

    // ��e�N�Z������U���W��+9�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    Out.tex4.xy = float2(9.0f / texSize.x, 0.0f);

    // ��e�N�Z������U���W��+11�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    Out.tex5.xy = float2(11.0f / texSize.x, 0.0f);

    // ��e�N�Z������U���W��+13�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    Out.tex6.xy = float2(13.0f / texSize.x, 0.0f);

    // ��e�N�Z������U���W��+15�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    Out.tex7.xy = float2(15.0f / texSize.x, 0.0f);

    // �I�t�Z�b�g��-1���|���ă}�C�i�X�����̃I�t�Z�b�g���v�Z����
    Out.tex0.zw = Out.tex0.xy * -1.0f;
    Out.tex1.zw = Out.tex1.xy * -1.0f;
    Out.tex2.zw = Out.tex2.xy * -1.0f;
    Out.tex3.zw = Out.tex3.xy * -1.0f;
    Out.tex4.zw = Out.tex4.xy * -1.0f;
    Out.tex5.zw = Out.tex5.xy * -1.0f;
    Out.tex6.zw = Out.tex6.xy * -1.0f;
    Out.tex7.zw = Out.tex7.xy * -1.0f;

    // �I�t�Z�b�g�Ɋ�e�N�Z����UV���W�𑫂��Z���āA
    // ���ۂɃT���v�����O����UV���W�ɕϊ�����
    Out.tex0 += float4(tex, tex);
    Out.tex1 += float4(tex, tex);
    Out.tex2 += float4(tex, tex);
    Out.tex3 += float4(tex, tex);
    Out.tex4 += float4(tex, tex);
    Out.tex5 += float4(tex, tex);
    Out.tex6 += float4(tex, tex);
    Out.tex7 += float4(tex, tex);

    return Out;
}