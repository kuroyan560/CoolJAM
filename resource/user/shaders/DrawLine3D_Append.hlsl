#include"../../engine/Camera.hlsli"

cbuffer cbuff0 : register(b0)
{
    Camera cam;
}

struct VSInput
{
    float4 from : FROM_POS;
    float4 to : TO_POS;
    float4 color : COLOR;
    float thickness : THICKNESS;
    float4 drawRate : DRAW_RATE;
};

VSInput VSmain(VSInput input)
{
    return input;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float mainDrawRate : MAIN;
    float emissiveDrawRate : EMISSIVE;
    float depthDrawRate : DEPTH;
};

//VecAÇVecBÇ…åXÇØÇÈâÒì]çsóÒ
matrix Rotate(float3 VecA, float3 VecB)
{
    float3 a = VecA;
    float3 b = VecB;
    a = normalize(a);
    b = normalize(b);
    float4 q = float4(0, 0, 0, 0);
    float3 c = cross(a, b);
    float d = length(c);
    c = normalize(c);
    
    float epsilon = 0.0002f;
    float ip = dot(a, b);
    
    if (-epsilon < d || 1.0f < ip)
    {
        if (ip < (epsilon - 1.0f))
        {
            float3 a2 = float3(-a.y, a.z, a.x);
            c = normalize(cross(a2, a));
            q.x = c.x;
            q.y = c.y;
            q.z = c.z;
            q.w = 0.0f;
        }
        else
        {
            q = float4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    else
    {
        float3 e = c * sqrt(0.5f * (1.0f - ip));
        q.x = e.x;
        q.y = e.y;
        q.z = e.z;
        q.w = sqrt(0.5f * (1.0f + ip));
    }
    
    matrix result;
    result[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
    result[0][1] = 2 * q.x * q.y + 2 * q.w * q.z;
    result[0][2] = 2 * q.x * q.z - 2 * q.w * q.y;
    result[0][3] = 0.0f;
    
    result[1][0] = 2 * q.x * q.y - 2 * q.w * q.z;
    result[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
    result[1][2] = 2 * q.y * q.z + 2 * q.w * q.x;
    result[1][3] = 0.0f;
    
    result[2][0] = 2 * q.x * q.z + 2 * q.w * q.y;
    result[2][1] = 2 * q.y * q.z - 2 * q.w * q.x;
    result[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
    result[2][3] = 0.0f;
    
    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;
    
    return result;
};

float4 GetPos(float4 Pos, float3 Offset, matrix Rotate)
{
    float4 offset = float4(Offset, 0.0f);
    float4 p = Pos + mul(Rotate, offset);
    return mul(cam.proj, mul(cam.view, p));
}

[maxvertexcount(6 * 6)] // 6ñ 
void GSmain(
    point VSInput input[1],
inout TriangleStream<GSOutput> output)
{
    GSOutput element;
    
    float offset = input[0].thickness / 2.0f;
    
    float4 from = input[0].from;
    float4 to = input[0].to;
    float4 p = from;
    
    element.col = input[0].color;
    element.mainDrawRate = input[0].drawRate.x;
    element.emissiveDrawRate = input[0].drawRate.y;
    element.depthDrawRate = input[0].drawRate.z;
    
    matrix rotate = Rotate(float3(0, 1, 0), to.xyz - from.xyz);
    
    //éËëO
    {
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);

        output.RestartStrip();
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
        
        output.RestartStrip();
    }
    //âú
    {
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);

        output.RestartStrip();
    
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
        
        output.RestartStrip();
    }
    //ç∂
    {
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);

        output.RestartStrip();
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);
        
        output.RestartStrip();
    }
    //âE
    {
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);

        output.RestartStrip();
    
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
        
        output.RestartStrip();
    }
    //è„ñ 
    {
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);

        output.RestartStrip();
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);
        
        output.RestartStrip();
    }
    //â∫ñ 
    {
        element.pos = GetPos(from, float3(offset, 0.0f, offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);

        output.RestartStrip();
    
        element.pos = GetPos(from, float3(offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, -offset), rotate);
        output.Append(element);
    
        element.pos = GetPos(from, float3(-offset, 0.0f, offset), rotate);
        output.Append(element);
        
        output.RestartStrip();
    }
}

struct PSOutput
{
    float4 color : SV_Target0;
    float4 emissive : SV_Target1;
    float depth : SV_Target2;
};

PSOutput PSmain(GSOutput input)
{
    PSOutput output;
    output.color = input.col * input.mainDrawRate;
    output.emissive = input.col * input.emissiveDrawRate;
    return output;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}