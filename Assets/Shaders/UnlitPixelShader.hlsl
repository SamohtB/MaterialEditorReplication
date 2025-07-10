Texture2D Textures[] : register(t0, space0);
SamplerState Samplers[] : register(s0, space0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer MaterialConstants : register(b2)
{
    uint diffuseHandleIndex;
    uint normalHandleIndex;
    uint metalHandleIndex;
    uint roughHandleIndex;

    uint aoHandleIndex;
    uint emmissiveHandleIndex;
    uint heightHandleIndex;
    uint materialFlags;

    float4 baseColor;

    float normalStr;
    float metalStr;
    float roughStr;
    float aoStr;

    float emmissiveStr;
    float heightStr;
    
    float2 tiling;
    float2 offset;
    
    float2 pad;
};

static const uint HasAlbedoMap = 1 << 0;

float4 PSMain(PSINPUT input) : SV_TARGET
{
    float3 color;
    float2 uv = input.texcoord / tiling + offset;
    
    if ((materialFlags & HasAlbedoMap) != 0)
    {
        float3 albedoTex = Textures[diffuseHandleIndex].Sample(Samplers[0], uv).rgb;
        color = pow(albedoTex, 2.2f) * baseColor.rgb;
        color = pow(color, 1.0 / 2.2);
    }
    else
    {
        color = baseColor.rgb;
    }

    return float4(color, 1.0);
}