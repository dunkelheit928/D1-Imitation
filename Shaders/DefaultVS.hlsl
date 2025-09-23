cbuffer Consts : register(b0)
{
    Matrix proj;
};

struct VSInput
{
    float3 pos : POSITION;
    float2 uv : UV;
};

struct VSInstance
{
    float4 posAndSize : INSTPOS;
    float4 uv : INSTUV;
    float4 color : INSTCOLOR;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VSOutput VSMain(VSInput input, VSInstance instance)
{
    VSOutput output;
    
    float2 local = input.pos.xy * instance.posAndSize.zw + instance.posAndSize.xy;
    output.pos = mul(float4(local.xy, 0.0, 1.0), proj);
    output.uv = input.uv;
    return output;
}