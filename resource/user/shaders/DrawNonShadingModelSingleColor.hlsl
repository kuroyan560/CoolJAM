#include"../../engine/ModelInfo.hlsli"
#include"../../engine/Camera.hlsli"

cbuffer cbuff0 : register(b0)
{
    Camera cam;
}

cbuffer cbuff1 : register(b1)
{
    matrix world;
}

cbuffer cbuff2 : register(b2)
{
    matrix bones[256]; //�{�[���s��
}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cbuff3 : register(b3)
{
    Material material;
}

cbuffer cbuff3 : register(b4)
{
    float4 color;
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VSmain(Vertex input)
{
    float4 resultPos = input.pos;
	
	//�{�[���s��
	//BDEF4(�{�[��4����3�ƁA���ꂼ��̃E�F�C�g�l�B�E�F�C�g���v��1.0�ł���ۏ�͂��Ȃ�)
    if (input.boneNo[2] != NO_BONE)
    {
        int num;
        
        if (input.boneNo[3] != NO_BONE)    //�{�[���S��
        {
            num = 4;
        }
        else //�{�[���R��
        {
            num = 3;
        }
        
        matrix mat = bones[input.boneNo[0]] * input.weight[0];
        for (int i = 1; i < num; ++i)
        {
            mat += bones[input.boneNo[i]] * input.weight[i];
        }
        resultPos = mul(mat, input.pos);
    }
	//BDEF2(�{�[��2�ƁA�{�[��1�̃E�F�C�g�l(PMD����))
    else if (input.boneNo[1] != NO_BONE)
    {
        matrix mat = bones[input.boneNo[0]] * input.weight[0];
        mat += bones[input.boneNo[1]] * (1 - input.weight[0]);
        
        resultPos = mul(mat, input.pos);
    }
	//BDEF1(�{�[���̂�)
    else if (input.boneNo[0] != NO_BONE)
    {
        resultPos = mul(bones[input.boneNo[0]], input.pos);
    }
	
    VSOutput output;
    output.svpos = mul(cam.proj, mul(cam.view, mul(world, resultPos)));
    output.uv = input.uv;
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
    output.color = color;
    return output;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}