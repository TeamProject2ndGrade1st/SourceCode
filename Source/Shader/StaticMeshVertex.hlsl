#include "StaticMesh.hlsli"

VS_OUT main(VSIN vin)
{
	VS_OUT vout;
    float sigma = vin.tangent.w;
    vin.normal.w = 0;
    vin.tangent.w = 0;
	row_major float4x4 viewProjection = mul(view, projection);
	vout.position = mul(vin.position, mul(world, viewProjection));
	vout.worldPosition = mul(vin.position, world);

    vout.worldNormal = normalize(mul(vin.normal, world));
    vout.worldTangent = normalize(mul(vin.tangent, world));
    vout.worldTangent.w = sigma;
	vout.texcoord = vin.texcoord;
	vout.color = materialColor;
	return vout;
}