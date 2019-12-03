
#include "common.hlsli"


float4 main(PS_INPUT_PCN input) : SV_TARGET
{
	float3 normal = normalize(input.normal_ws);
	float4 diffuse = float4(input.diffuse, 1.f);

	float4 final_color;
	final_color = diffuse * light.ambient;
	final_color += saturate(dot(light.direction, normal) * light.diffuse * diffuse);

	return float4(final_color.rgb, diffuse.a);
}
