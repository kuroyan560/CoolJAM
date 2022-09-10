#include"../engine/ModelInfo.hlsli"
#include"../engine/Camera.hlsli"
#include"../engine/LightInfo.hlsli"

cbuffer cbuff0 : register(b0)
{
    Camera cam;
}

cbuffer cbuff1 : register(b1)
{
    LightInfo ligNum; //アクティブ中のライトの数の情報
}

StructuredBuffer<DirectionLight> dirLight : register(t0);
StructuredBuffer<PointLight> pointLight : register(t1);
StructuredBuffer<SpotLight> spotLight : register(t2);
StructuredBuffer<HemiSphereLight> hemiSphereLight : register(t3);


cbuffer cbuff2 : register(b2)
{
    matrix world;
    float instanceAlpha;
    bool drawMainFlg;
    bool drawEmissiveFlg;
    bool drawDepthFlg;
}

cbuffer cbuff3 : register(b3)
{
    matrix bones[256]; //ボーン行列
}

Texture2D<float4> colorTex : register(t4);
Texture2D<float4> emissiveMap : register(t5);
SamplerState smp : register(s0);

cbuffer cbuff4 : register(b4)
{
    Material material;
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 worldpos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
	float depthInView : CAM_Z;
    
};

VSOutput VSmain(Vertex input)
{
    float4 resultPos = input.pos;
	
	//ボーン行列
	//BDEF4(ボーン4つ又は3つと、それぞれのウェイト値。ウェイト合計が1.0である保障はしない)
    if (input.boneNo[2] != NO_BONE)
    {
        int num;
        
        if (input.boneNo[3] != NO_BONE)    //ボーン４つ
        {
            num = 4;
        }
        else //ボーン３つ
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
	//BDEF2(ボーン2つと、ボーン1のウェイト値(PMD方式))
    else if (input.boneNo[1] != NO_BONE)
    {
        matrix mat = bones[input.boneNo[0]] * input.weight[0];
        mat += bones[input.boneNo[1]] * (1 - input.weight[0]);
        
        resultPos = mul(mat, input.pos);
    }
	//BDEF1(ボーンのみ)
    else if (input.boneNo[0] != NO_BONE)
    {
        resultPos = mul(bones[input.boneNo[0]], input.pos);
    }
	
    VSOutput output;
    float4 wpos = mul(world, resultPos); //ワールド変換
    output.svpos = mul(cam.view, wpos); //ビュー変換
	output.depthInView = output.svpos.z;	//カメラから見たZ
    output.svpos = mul(cam.proj, output.svpos); //プロジェクション変換
    output.worldpos = wpos;
    output.normal = normalize(mul(world, input.normal));
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
    output.color = float4(0, 0, 0, 0);
    output.emissive = float4(0, 0, 0, 0);
    output.depth = 0.0f;

    if(drawMainFlg)
    {
        float3 normal = input.normal;
        float3 vnormal = normalize(mul(cam.view, normal));
    
     //ライトの影響
        float3 ligEffect = { 0.0f, 0.0f, 0.0f };
    
    //ディレクションライト
        for (int i = 0; i < ligNum.dirLigNum; ++i)
        {
            if (!dirLight[i].active)
                continue;
        
            float3 dir = dirLight[i].direction;
            float3 ligCol = dirLight[i].color.xyz * dirLight[i].color.w;
            ligEffect += CalcLambertDiffuse(dir, ligCol, normal) * (material.diffuse * material.diffuseFactor);
            ligEffect += CalcPhongSpecular(dir, ligCol, normal, input.worldpos, cam.eyePos) * (material.specular * material.specularFactor);
            ligEffect += CalcLimLight(dir, ligCol, normal, vnormal);
        }
    //ポイントライト
        for (int i = 0; i < ligNum.ptLigNum; ++i)
        {
            if (!pointLight[i].active)
                continue;
        
            float3 dir = input.worldpos - pointLight[i].pos;
            dir = normalize(dir);
            float3 ligCol = pointLight[i].color.xyz * pointLight[i].color.w;
        
        //減衰なし状態
            float3 diffPoint = CalcLambertDiffuse(dir, ligCol, normal);
            float3 specPoint = CalcPhongSpecular(dir, ligCol, normal, input.worldpos, cam.eyePos);
        
        //距離による減衰
            float3 distance = length(input.worldpos - pointLight[i].pos);
		//影響率は距離に比例して小さくなっていく
            float affect = 1.0f - 1.0f / pointLight[i].influenceRange * distance;
		//影響力がマイナスにならないように補正をかける
            if (affect < 0.0f)
                affect = 0.0f;
		//影響を指数関数的にする
            affect = pow(affect, 3.0f);
            diffPoint *= affect;
            specPoint *= affect;
        
            ligEffect += diffPoint * (material.diffuse * material.diffuseFactor);
            ligEffect += specPoint * (material.specular * material.specularFactor);
            ligEffect += CalcLimLight(dir, ligCol, normal, vnormal);
        }
    //スポットライト
        for (int i = 0; i < ligNum.spotLigNum; ++i)
        {
            if (!spotLight[i].active)
                continue;
        
            float3 ligDir = input.worldpos - spotLight[i].pos;
            ligDir = normalize(ligDir);
            float3 ligCol = spotLight[i].color.xyz * spotLight[i].color.w;
        
        //減衰なし状態
            float3 diffSpotLight = CalcLambertDiffuse(ligDir, ligCol, normal);
            float3 specSpotLight = CalcPhongSpecular(ligDir, ligCol, normal, input.worldpos, cam.eyePos);
        
        //スポットライトとの距離を計算
            float3 distance = length(input.worldpos - spotLight[i].pos);
       	//影響率は距離に比例して小さくなっていく
            float affect = 1.0f - 1.0f / spotLight[i].influenceRange * distance;
        //影響力がマイナスにならないように補正をかける
            if (affect < 0.0f)
                affect = 0.0f;
    //影響を指数関数的にする
            affect = pow(affect, 3.0f);
            diffSpotLight *= affect;
            specSpotLight *= affect;
    
            float3 spotlim = CalcLimLight(ligDir, ligCol, normal, vnormal) * affect;
        
            float3 dir = normalize(spotLight[i].target - spotLight[i].pos);
            float angle = dot(ligDir, dir);
            angle = abs(acos(angle));
            affect = 1.0f - 1.0f / spotLight[i].angle * angle;
            if (affect < 0.0f)
                affect = 0.0f;
            affect = pow(affect, 0.5f);
        
            ligEffect += diffSpotLight * affect * (material.diffuse * material.diffuseFactor);
            ligEffect += specSpotLight * affect * (material.specular * material.specularFactor);
            ligEffect += spotlim * affect;
        }
    //天球
        for (int i = 0; i < ligNum.hemiSphereNum; ++i)
        {
            if (!hemiSphereLight[i].active)
                continue;
        
            float t = dot(normal.xyz, hemiSphereLight[i].groundNormal);
            t = (t + 1.0f) / 2.0f;
            float3 hemiLight = lerp(hemiSphereLight[i].groundColor, hemiSphereLight[i].skyColor, t);
            ligEffect += hemiLight;
        }
    
        float4 result = colorTex.Sample(smp, input.uv);
        result.xyz = (material.ambient + ligEffect) * result.xyz;
        result.w *= (1.0f - material.transparent);
        
        output.color = result;
    }
    
    output.color.w *= instanceAlpha;
    
    output.emissive = emissiveMap.Sample(smp, input.uv);
    output.emissive.xyz += material.emissive;
    output.emissive.w *= instanceAlpha;
    
    output.depth = input.depthInView;
    
    ////明るさ計算
    //float bright = dot(result.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    //if (1.0f < bright)
    //    output.emissive += result;
    //output.emissive.w = result.w;
    
    return output;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}