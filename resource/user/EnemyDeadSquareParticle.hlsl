#include"../engine/Camera.hlsli"
#include"../engine/Math.hlsli"

cbuffer cbuff0 : register(b0)
{
    Camera cam;
}

cbuffer cbuff1 : register(b1)
{
    float linePtThickness;
    float squarePtSize;
    float emissiveStrongth;
}

Texture2D<float4> colorTex : register(t0);
SamplerState smp : register(s0);

struct VSInput
{
    float3 m_angle : ANGLE;
    int m_initFlg : INIT_FLG;
    float3 m_angleVel : ANGLE_VEL;
    int m_alpha : ALPHA;
    float3 m_pos : POS;
    int m_disappearTime : DISAPPEAR_TIME;
    float3 m_vel : VEL;
};

VSInput VSmain(VSInput input)
{
    return input;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float alpha : ALPHA;
};

float4 GetPos(float3 Pos, float2 Offset, matrix RotateMat)
{
    float4 offset = float4(Offset, 0, 1);
    offset = mul(RotateMat, offset);
    float4 result = float4(Pos, 1);
    result.xyz += offset.xyz;
    result = mul(cam.proj, mul(cam.view, result));
    return result;
}

[maxvertexcount(4)]
void GSmain(
    point VSInput input[1],
inout TriangleStream<GSOutput> output)
{
    float2 offset = squarePtSize / 2.0f;
    
    GSOutput element;
    element.alpha = input[0].m_alpha;
    
    float4x4 rotate = GetRoateMat(input[0].m_angle);
    

    //ç∂â∫
    element.pos = GetPos(input[0].m_pos, float2(-offset.x, -offset.y), rotate);
    element.uv = float2(0, 1);
    output.Append(element);
    
    //ç∂è„
    element.pos = GetPos(input[0].m_pos, float2(-offset.x, offset.y), rotate);
    element.uv = float2(0, 0);
    output.Append(element);
    
    //âEâ∫
    element.pos = GetPos(input[0].m_pos, float2(offset.x, -offset.y), rotate);
    element.uv = float2(1, 1);
    output.Append(element);
    
    //âEè„
    element.pos = GetPos(input[0].m_pos, float2(offset.x, offset.y), rotate);
    element.uv = float2(1, 0);
    output.Append(element);
}


struct PSOutput
{
    float4 color : SV_Target0;
    float4 emissive : SV_Target1;
};

PSOutput PSmain(GSOutput input)
{
    PSOutput output;
    output.color = colorTex.Sample(smp, input.uv);
    output.emissive = output.color * emissiveStrongth;
    return output;
}