
struct VS_INPUT_PCN
{
	float3 position  : POSITION;
	float3 color     : COLOR;
	float3 normal    : NORMAL;
};


struct VS_OUTPUT_PCN
{
	float3 diffuse   : COLOR;
	float3 normal_ws : NORMAL;
	float4 position  : SV_POSITION;
};


struct PS_INPUT_PCN
{
	float3 diffuse   : COLOR;
	float3 normal_ws : NORMAL;
};


struct DIRECTIONAL_LIGHT
{
	float3 direction;
	float4 ambient;
	float4 diffuse;
};
