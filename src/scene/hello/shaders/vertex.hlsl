
#include "common.hlsli"

COLOR_VERTEX main(float4 position : POSITION, float4 color : COLOR)
{
    COLOR_VERTEX output;
    output.position = position;
    output.color = color;
    return output;
}