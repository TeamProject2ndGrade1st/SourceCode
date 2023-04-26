float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor,
	float3 eyeVector, float shininess, float3 ks)
{
    float3 R = reflect(lightVector, normal);

    float d = max(dot(-eyeVector, R), 0);
    d = pow(d, shininess);

    return d * lightColor * ks;
}
