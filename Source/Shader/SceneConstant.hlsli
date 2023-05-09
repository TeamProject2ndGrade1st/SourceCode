struct DirectionalLight
{
    float3 direction;
    float4 color;
};

struct PointLight
{
    float3 position;
    float range;
    float4 color;
};

static const int NumPointLight = 4;

cbuffer SceneConstant : register(b0)
{
	row_major float4x4 view;
	row_major float4x4 projection;
	float3 cameraPosition;
    DirectionalLight directionalLight;
    PointLight pointLight;
}
