

struct INPUT_VERTEX_FORMAT
{
	float3 pos : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
};


struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
	float3 normal : NORMAL;
};
