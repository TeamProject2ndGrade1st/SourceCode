float3 CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
    float d = max(0, dot(normal, -lightVector));

    return kd * lightColor * d;
}

float3 CalcDiffuseColor(float3 normal, float3 lightVector, float3 lightColor, float3 kd, float3 color)
{
    float3 diffuse = CalcLambertDiffuse(normal, lightVector, lightColor, kd);
    float3 col = color.rgb * diffuse.rgb;
    float3 ret = float3(
    clamp(col.r, 0, 1),
    clamp(col.g, 0, 1),
    clamp(col.b, 0, 1));
    return ret;
}