#include "SceneConstant.hlsli"
#include "DebugMesh.hlsli"

VSOut main(VSIn vin)
{
    VSOut vout;
    row_major float4x4 viewProjection = mul(view, projection);
    vout.position = mul(vin.position, mul(world, viewProjection));
    vout.normal = normalize(mul(vin.normal, world));
    vout.texcoord = vin.texcoord;
    return vout;
}