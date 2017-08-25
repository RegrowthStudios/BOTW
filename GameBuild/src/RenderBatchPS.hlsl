struct Input {
};

struct Output {
    float4 color : SV_TARGET;
};

Output main(Input input)
{
    Output output;

    output.color = float4(1.0f, 0.0f, 1.0f, 1.0f);

    return output;
}
