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
float Length(float2 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float2 Normalize(float2 vec)
{
    float length = Length(vec);
    float2 nomVec = vec;
    if (length > 0)
    {
        nomVec.x = nomVec.x / length;
        nomVec.y = nomVec.y / length;
    }
    return nomVec;
}

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
    color = tex.Sample(smp, input.uv);
    //UV
    float2 uv = input.uv;
 
    float d = length(uv - 0.5);

    float lerpRate = saturate(angle);
    float4 vignetteColor = lerp(color, float4(1.0, 0.0, 0.0, 1.0),lerpRate);

    float vignetteDistance = pow(saturate(d * 1.5), 3);

    color = lerp(color, vignetteColor, vignetteDistance);
    output.color = color;
    return output;
}