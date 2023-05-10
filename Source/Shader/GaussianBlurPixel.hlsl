#include "GaussianBlur.hlsli"


SamplerState smp : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	// step-15 X,Yブラー用のピクセルシェーダーを実装
    float4 Color;

    // 基準テクセルからプラス方向に8テクセル、重み付きでサンプリング
    Color =  weight[0].x * tex.Sample(smp, pin.tex0.xy);
    Color += weight[0].y * tex.Sample(smp, pin.tex1.xy);
    Color += weight[0].z * tex.Sample(smp, pin.tex2.xy);
    Color += weight[0].w * tex.Sample(smp, pin.tex3.xy);
    Color += weight[1].x * tex.Sample(smp, pin.tex4.xy);
    Color += weight[1].y * tex.Sample(smp, pin.tex5.xy);
    Color += weight[1].z * tex.Sample(smp, pin.tex6.xy);
    Color += weight[1].w * tex.Sample(smp, pin.tex7.xy);

    // 基準テクセルにマイナス方向に8テクセル、重み付きでサンプリング
    Color += weight[0].x * tex.Sample(smp, pin.tex0.zw);
    Color += weight[0].y * tex.Sample(smp, pin.tex1.zw);
    Color += weight[0].z * tex.Sample(smp, pin.tex2.zw);
    Color += weight[0].w * tex.Sample(smp, pin.tex3.zw);
    Color += weight[1].x * tex.Sample(smp, pin.tex4.zw);
    Color += weight[1].y * tex.Sample(smp, pin.tex5.zw);
    Color += weight[1].z * tex.Sample(smp, pin.tex6.zw);
    Color += weight[1].w * tex.Sample(smp, pin.tex7.zw);

    return float4(Color.xyz, 1.0f);
}