/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
/// <remark>
/// この関数はフレネル反射を考慮した拡散反射率を計算します
/// フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります
/// 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです
/// つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります
///
/// </remark>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
/// <param name="roughness">粗さ。0～1の範囲。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-1 ディズニーベースのフレネル反射による拡散反射を真面目に実装する。
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