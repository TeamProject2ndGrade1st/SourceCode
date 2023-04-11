
struct VSIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

cbuffer ObjectConstant : register(b1)
{
    row_major float4x4 world;
}