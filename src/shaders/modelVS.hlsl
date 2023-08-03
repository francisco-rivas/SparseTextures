#include "model.hlsli"

VSOutput main(float4 pos : POSITION, float4 uv : TEXCOORD0)
{
    VSOutput result;
    
    result.position = pos;
    result.uv = uv.rg;
    
    return result;
}