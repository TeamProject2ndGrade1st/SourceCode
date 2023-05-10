#include "FullscreenQuad.hlsli"

Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
Texture2D tex3 : register(t3);

SamplerState smp : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 combineColor = tex0.Sample(smp, pin.texcoord);
    combineColor += tex1.Sample(smp, pin.texcoord);
    combineColor += tex2.Sample(smp, pin.texcoord);
    combineColor += tex3.Sample(smp, pin.texcoord);
    combineColor /= 4.0f;
    combineColor.a = 1.0f;
    return combineColor;
}