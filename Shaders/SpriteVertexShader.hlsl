cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.texcoord = input.texcoord;

    return output;
}