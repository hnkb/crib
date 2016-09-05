
#include "shared.hlsli"


float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(input.color, 1.f);
}
