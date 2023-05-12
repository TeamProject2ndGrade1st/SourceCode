#include "FullscreenQuad.hlsli"
#include "SceneConstant.hlsli"


SamplerState smp : register(s0);
Texture2D skybox : register(t0);

//todo 後ほど理解すること
float4 sample_skybox(float3 v, float roughness)
{
    const float PI = 3.14159265358979;

    uint width, height, number_of_levels;
    skybox.GetDimensions(0, width, height, number_of_levels);

    //多分ミップマップと下を使うよう？　おそらく
    float lod = roughness * float(number_of_levels - 1);
    v = normalize(v);

	//zとxからタンジェントを求めそれを角度に変換する　その場合は範囲は
    //-pi ~ piになる　uv座標は0~1の範囲に収めたいのでいい感じの計算式を使う
    //yの場合はサインから角度を求め、真上向いたときはPI(このときのV座標は０）
    //真下を向いたときは-pI(このときのV座標は1)になるようないい感じの計算式を使う
    float2 sample_point;
    sample_point.x = (atan2(v.z, v.x) + PI) / (PI * 2.0);
    sample_point.y = 1.0 - ((asin(v.y) + PI * 0.5) / PI);
    
    return skybox.SampleLevel(smp, sample_point, lod);
}

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 ndc;
    //UV座標を用いてNDC座標系に変換する 背景の場合は一番奥に描画されるため
    //Z値は１
    ndc.x = (pin.texcoord.x * 2.0) - 1.0;
    ndc.y = 1.0 - (pin.texcoord.y * 2.0);
    ndc.z = 1;
    ndc.w = 1;

    //ビュープロジェクション行列の逆行列をかけることでワールド座標へ変換
    float4 R = mul(ndc, invViewProjection);

    //これはなんですか？？？
    R /= R.w;
    const float roughness = 0;
    return sample_skybox(R.xyz, roughness);
}
