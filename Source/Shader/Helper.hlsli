//すべて正規化とかをする必要はない　texNormalはサンプリングしたものをそのまま渡すこと
//
float3 GetNormal(float3 surfaceNormal, float3 texNormal, float4 worldTangent)
{
    const float3 N = normalize(surfaceNormal);
    const float sigma = worldTangent.w;
    float3 T = normalize(worldTangent.xyz);
    T = normalize(T - N * dot(N, T));
    const float3 B = normalize(cross(N, T) * sigma);
    texNormal = (texNormal * 2.0f) - 1.0f;
    float3 ret = normalize(texNormal.x * T + texNormal.y * B + texNormal.z * N);

    return ret;
}