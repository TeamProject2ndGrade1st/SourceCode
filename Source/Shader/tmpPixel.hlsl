#include "FullscreenQuad.hlsli"

Texture2D tex1 : register(t0);
Texture2D tex2 : register(t1);
SamplerState smp : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 ret;
    //if(pin.texcoord.x > 0.5f)
    //{
    //    ret = tex1.Sample(smp, pin.texcoord);
    //}
    //else
    //{
    //    ret = tex2.Sample(smp, pin.texcoord);
    //}

    ret = tex1.Sample(smp, pin.texcoord);
    ret = tex2.Sample(smp, pin.texcoord);
	return ret;
}