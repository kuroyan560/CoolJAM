#include"../../engine/Camera.hlsli"
#include"../../engine/Math.hlsli"

cbuffer cbuff0 : register(b0)
{
    Camera cam;
}

cbuffer cbuff1 : register(b1)
{
    matrix world;
    float3 drawRate;
}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0); //０番スロットに設定されたサンプラー

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    float4 resultPos = pos;
	
    VSOutput output;
    float4 wpos = mul(world, resultPos); //ワールド変換
    output.svpos = mul(cam.view, wpos); //ビュー変換
    output.svpos = mul(cam.proj, output.svpos); //プロジェクション変換
    output.uv = uv;
    return output;
}

struct PSOutput
{
    float4 color : SV_Target0;
    float4 emissive : SV_Target1;
    float depth : SV_Target2;
};

PSOutput PSmain(VSOutput input) : SV_TARGET
{
    PSOutput output;
    output.color = float4(0, 0, 0, 0);
    output.emissive = float4(0, 0, 0, 0);
    output.depth = 0.0f;
    
    float4 colorTexCol = tex.Sample(smp, input.uv);

    output.color = colorTexCol;
    output.color.w *= drawRate.x;
    
    if(any(drawRate.y))
    {
        output.emissive = output.color;
        output.emissive.w = drawRate.y;
    }
        
    if(any(drawRate.z))
    {
    }
    return output;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}