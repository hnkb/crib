
#include "shared.hlsli"


cbuffer CB_PS_PERFRAME : register(b0)
{
	DIRECTIONAL_LIGHT light;
};


float4 main(PS_INPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 diffuse = float4(input.color, 1.f);

	float4 final_color;
	final_color = diffuse * light.ambient;
	final_color += saturate(dot(light.direction, input.normal) * light.diffuse * diffuse);
	
	return float4(final_color.rgb, diffuse.a);
}
