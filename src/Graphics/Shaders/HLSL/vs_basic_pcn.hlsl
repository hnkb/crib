
#include "common.hlsli"


VS_OUTPUT_PCN main(VS_INPUT_PCN input)
{
	float4 pos = float4(input.position, 1.f);
	pos = mul(pos, world_view_proj);

	float4 normal = float4(input.normal, 0.f);
	normal = mul(normal, world);

	VS_OUTPUT_PCN output;
	output.diffuse = input.color;
	output.normal_ws = normal.xyz;
	output.position = pos;
	return output;
}
