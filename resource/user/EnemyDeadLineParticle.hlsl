#include"../engine/Camera.hlsli"

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

struct VSInput
{
    float3 m_pos : POS;
    int m_initFlg : INIT_FLG;
    float3 m_vel : VEL;
    float m_length : LEN;
    float3 m_startPos : START_POS;
    int m_alpha : ALPHA;
    float3 m_endPos : END_POS;
    int m_disappearTime : DISAPPEAR_TIME;
    int3 m_color : COLOR;
};

VSInput VSmain(VSInput input)
{
    return input;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
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

GSOutput Get(float4 Pos, float3 Offset, matrix Rotate, float4 Color)
{
    GSOutput element;
    
    float4 offset = float4(Offset, 0.0f);
    float4 p = Pos + mul(Rotate, offset);
    element.pos = mul(cam.proj, mul(cam.view, p));
    
    element.col = Color;
    return element;
}

[maxvertexcount(6 * 6)] // 6ñ 
void GSmain(
    point VSInput input[1],
inout TriangleStream<GSOutput> output)
{
    if (!input[0].m_initFlg)
        return;
    
    float offset = linePtThickness / 2.0f;
    float4 col;
    col.r = input[0].m_color.r / 255.0f;
    col.g = input[0].m_color.r / 255.0f;
    col.b = input[0].m_color.r / 255.0f;
    col.a = input[0].m_alpha / 255.0f;
    
    float4 from = float4(input[0].m_startPos, 1.0f);
    float4 to = float4(input[0].m_endPos, 1.0f);
    float4 p = from;
    
    matrix rotate = Rotate(float3(0, 1, 0), to.xyz - from.xyz);
    
    //éËëO
    {
        output.Append(Get(from, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(from, float3(-offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, -offset), rotate, col));

        output.RestartStrip();
    
        output.Append(Get(to, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(from, float3(offset, 0.0f, -offset), rotate, col));
        
        output.RestartStrip();
    }
    //âú
    {
        output.Append(Get(from, float3(offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(from, float3(-offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, offset), rotate, col));

        output.RestartStrip();
    
        output.Append(Get(to, float3(offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(from, float3(offset, 0.0f, offset), rotate, col));
        
        output.RestartStrip();
    }
    //ç∂
    {
        output.Append(Get(from, float3(-offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(from, float3(-offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, -offset), rotate, col));

        output.RestartStrip();
    
        output.Append(Get(to, float3(-offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(from, float3(-offset, 0.0f, offset), rotate, col));
        
        output.RestartStrip();
    }
    //âE
    {
        output.Append(Get(from, float3(offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(from, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(to, float3(offset, 0.0f, -offset), rotate, col));

        output.RestartStrip();
    
        output.Append(Get(to, float3(offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(to, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(from, float3(offset, 0.0f, offset), rotate, col));
        
        output.RestartStrip();
    }
    //è„ñ 
    {
        output.Append(Get(from, float3(offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(from, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(from, float3(-offset, 0.0f, offset), rotate, col));

        output.RestartStrip();
    
        output.Append(Get(from, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(from, float3(-offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(from, float3(-offset, 0.0f, offset), rotate, col));
        
        output.RestartStrip();
    }
    //â∫ñ 
    {
        output.Append(Get(to, float3(offset, 0.0f, offset), rotate, col));
    
        output.Append(Get(to, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, offset), rotate, col));

        output.RestartStrip();
    
        output.Append(Get(to, float3(offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, -offset), rotate, col));
    
        output.Append(Get(to, float3(-offset, 0.0f, offset), rotate, col));
        
        output.RestartStrip();
    }
}

struct PSOutput
{
    float4 color : SV_Target0;
    float4 emissive : SV_Target1;
};

PSOutput PSmain(GSOutput input)
{
    PSOutput output;
    output.color = input.col;
    output.emissive = input.col * emissiveStrongth;
    return output;
}