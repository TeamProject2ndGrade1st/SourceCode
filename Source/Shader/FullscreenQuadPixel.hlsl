#include "FullscreenQuad.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
float4 main(VS_OUT pin) : SV_TARGET
{
	return tex.Sample(smp, pin.texcoord);
}