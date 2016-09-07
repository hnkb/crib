
#include "shared.hlsli"


cbuffer CB_VS_PEROBJECT : register(b0)
{
    matrix world;
    matrix wvp; // world * view * projection
};


PS_INPUT main(INPUT_VERTEX_FORMAT input)
{
    float4 pos = float4(input.pos, 1.f);
    pos = mul(pos, wvp);

	// Calculate normal in world coordinates to be used by PS in lighting
	float4 normal = float4(input.normal, 0.f);
	normal = mul(normal, world);

	PS_INPUT output;
	output.pos = pos;
	output.color = input.color;
	output.normal = normal.xyz;
	return output;
}
