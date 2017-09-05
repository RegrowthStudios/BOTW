cbuffer Constants : register(b0) {
    float4x4 worldViewProjection;
};

struct Input {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 tint : COLOR0;
};

struct Output {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 tint : COLOR0;
};

Output main(Input input)
{
    Output output;
    
    output.position = mul(worldViewProjection, input.position);
    output.uv = input.uv;
    output.tint = input.tint;

    return output;
}
