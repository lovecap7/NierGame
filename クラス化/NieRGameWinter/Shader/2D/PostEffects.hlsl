SamplerState smp : register(s0);
Texture2D tex : register(t0);
Texture2D norm : register(t1); //テクスチャ
cbuffer ConstantBuffer : register(b4)
{
    float angle;
    float blockScale;
    float noiseSpeed;
    float dummy;
}
//モノクロにするための準備
#define R_LUMINANCE 0.298912
#define G_LUMINANCE 0.586611
#define B_LUMINANCE 0.114478

float random(float2 seeds)
{
    return frac(sin(dot(seeds, float2(12.9898, 78.233))) * 43758.5453);
}

float blockNoise(float2 seeds)
{
    return random(floor(seeds));
}

float noiserandom(float2 seeds)
{
    return -1.0 + 2.0 * blockNoise(seeds);
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 dif : COLOR0;
    float4 spc : COLOR1;
    float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};


PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    float4 color;
   
  
    ///グリッジ///
    
    float2 gv = input.uv;
    float noise = blockNoise(input.uv.y * blockScale);
    noise += random(input.uv.x) * 0.3;
    float2 randomvalue = noiserandom(float2(input.uv.y, angle * noiseSpeed));
    gv.x += randomvalue * sin(sin(1) * 0.5) * sin(-sin(noise) * 0.2) * frac(angle);
    color.r = tex.Sample(smp, gv + float2(0.006, 0)).r;
    color.g = tex.Sample(smp, gv).g;
    color.b = tex.Sample(smp, gv - float2(0.008, 0)).b;
    color.a = 1.0;
    
    float4 beforeColor = color;
    
    ///モノクロ処理///
    
    // 輝度を計算 (モノクロ化)
    float luminance = dot(color.rgb, float3(R_LUMINANCE, G_LUMINANCE, B_LUMINANCE));
    // モノクロのRGBAに変換
    color = float4(luminance, luminance, luminance, 1.0);
    color = lerp(beforeColor, color, 0.9);
    color.g *= 1.07;
    color.b *= 1.04;
    
    output.color = color;
    return output;
}