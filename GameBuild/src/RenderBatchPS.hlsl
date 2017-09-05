struct Input {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 tint : COLOR0;
};

struct Output {
    float4 color : SV_TARGET;
};

Output main(Input input)
{
    Output output;

    output.color = input.tint;

    return output;
}
