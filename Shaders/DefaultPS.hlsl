Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

float4 PSMain(PSInput input) : SV_Target
{
    return tex0.Sample(samp0, input.uv);
}