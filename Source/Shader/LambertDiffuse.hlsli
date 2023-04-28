float3 CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
    float d = max(0, dot(normal, -lightVector));

    return kd * lightColor * d;
}
