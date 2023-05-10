 #include "FullscreenQuad.hlsli"

SamplerState samplerStates : register(s0);

Texture2D textureMaps : register(t0);

float4 main(VS_OUT pin) : SV_TARGET
 {

    float4 color = textureMaps.Sample(samplerStates, pin.texcoord);
    
    float alpha = color.a;
 	color.rgb = smoothstep(0.6, 0.8, dot(color.rgb, float3(0.299, 0.587, 0.114))) * color;
 	return float4(color.rgb, alpha);
}