#include "GaussianBlur.hlsli"

VS_OUT main(in uint vertexId : SV_VERTEXID)
{
    VS_OUT Out;

    // 座標変換
    const float2 position[4] = { { -1, +1 }, { +1, +1 }, { -1, -1 }, { +1, -1 } };
    
    const float2 texcoords[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    
    Out.position = float4(position[vertexId], 0, 1);


    // テクスチャサイズを取得
    float2 texSize;
    float level;
    tex.GetDimensions(0, texSize.x, texSize.y, level);

    // 基準テクセルのUV座標を記録
    float2 tex = texcoords[vertexId];

    // 基準テクセルからU座標を+1テクセルずらすためのオフセットを計算する
    Out.tex0.xy = float2(1.0f / texSize.x, 0.0f);

    // 基準テクセルからU座標を+3テクセルずらすためのオフセットを計算する

    Out.tex1.xy = float2(3.0f / texSize.x, 0.0f);

    // 基準テクセルからU座標を+5テクセルずらすためのオフセットを計算する
    Out.tex2.xy = float2(5.0f / texSize.x, 0.0f);

    // 基準テクセルからU座標を+7テクセルずらすためのオフセットを計算する
    Out.tex3.xy = float2(7.0f / texSize.x, 0.0f);

    // 基準テクセルからU座標を+9テクセルずらすためのオフセットを計算する
    Out.tex4.xy = float2(9.0f / texSize.x, 0.0f);

    // 基準テクセルからU座標を+11テクセルずらすためのオフセットを計算する
    Out.tex5.xy = float2(11.0f / texSize.x, 0.0f);

    // 基準テクセルからU座標を+13テクセルずらすためのオフセットを計算する
    Out.tex6.xy = float2(13.0f / texSize.x, 0.0f);

    // 基準テクセルからU座標を+15テクセルずらすためのオフセットを計算する
    Out.tex7.xy = float2(15.0f / texSize.x, 0.0f);

    // オフセットに-1を掛けてマイナス方向のオフセットも計算する
    Out.tex0.zw = Out.tex0.xy * -1.0f;
    Out.tex1.zw = Out.tex1.xy * -1.0f;
    Out.tex2.zw = Out.tex2.xy * -1.0f;
    Out.tex3.zw = Out.tex3.xy * -1.0f;
    Out.tex4.zw = Out.tex4.xy * -1.0f;
    Out.tex5.zw = Out.tex5.xy * -1.0f;
    Out.tex6.zw = Out.tex6.xy * -1.0f;
    Out.tex7.zw = Out.tex7.xy * -1.0f;

    // オフセットに基準テクセルのUV座標を足し算して、
    // 実際にサンプリングするUV座標に変換する
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