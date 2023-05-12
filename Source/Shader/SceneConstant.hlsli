struct DirectionalLight
{
    float4 direction;
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
	float4 cameraPosition;
    
    DirectionalLight directionalLight;
    PointLight pointLight[NumPointLight];
    row_major float4x4 invViewProjection;
}
