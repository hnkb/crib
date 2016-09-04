
#include "ps_input.hlsli"

cbuffer CB_LAYOUT : register(b0)
{
    matrix model;
    matrix camera; // projection * view
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 color : COLOR;
};


PS_INPUT main(VS_INPUT input)
{
    float4 pos = float4(input.pos, 1.0f);

    pos = mul(pos, model);
    pos = mul(pos, camera);
	
	PS_INPUT vertexShaderOutput;
	vertexShaderOutput.pos = pos;
    vertexShaderOutput.color = float4(input.color, 1.0f);

	return vertexShaderOutput;
}
