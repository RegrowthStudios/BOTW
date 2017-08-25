struct Input {
    float4 position : POSITION;
};

struct Output {
    float4 position : SV_POSITION;
};

Output main(Input input)
{
    Output output;
    
    output.position = input.position;

    return output;
}
