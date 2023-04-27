struct Light
{
    float4 color;
    float4 position;
};

cbuffer SceneConstant : register(b0)
{
	row_major float4x4 view;
	row_major float4x4 projection;
	float4 cameraPosition;
    Light light[2];
}
