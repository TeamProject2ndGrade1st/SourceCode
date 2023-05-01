#include "SceneConstant.hlsli"

struct VSIN
{
	float4 position : POSITION;
	float4 normal : NORMAL;
    float4 tangent : TANGENT;
	float2 texcoord : TEXCOORD;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 worldNormal : NORMAL;
    float4 worldTangent : TANGENT;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

cbuffer ObjectConstant : register(b1)
{
	row_major float4x4 world;
}

cbuffer MaterialConstant : register(b2)
{
	float4 color;
	float4 ka;
	float4 kd;
	float4 ks;
	float shininess;
}