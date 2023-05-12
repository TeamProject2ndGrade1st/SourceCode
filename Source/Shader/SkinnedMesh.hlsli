#include "SceneConstant.hlsli"

struct VSIN
{
	float4 position : POSITION;
	float4 normal : NORMAL;
    float4 tangent : TANGENT;
	float2 texcoord : TEXCOORD;
	float4 boneWeights : WEIGHTS;
	uint4 boneIndices : BONES;
};


struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 worldNormal : NORMAL;
    float4 worldTangent : TANGENT;
	float2 texcoord : TEXCOORD;
};

static const int MaxBones = 256;
cbuffer ObjectConstant : register(b1)
{
	row_major float4x4 world;
}
cbuffer MeshConstant : register(b2)
{
	row_major float4x4 globalTransform;
	row_major float4x4 boneTransforms[MaxBones];
}

cbuffer MaterialConstant : register(b3)
{
    float4 materialColor;
    float4 emissiveColor;
	float4 ka;
	float4 kd;
	float4 ks;
	float shininess;
}