Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer ColorBuffer : register(b0)
{
    float4 spriteColor;
};

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.texcoord) * spriteColor;
}