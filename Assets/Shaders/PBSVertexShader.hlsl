struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 tangentWS : TANGENT;
    float3 bitangentWS : BITANGENT;
    float3 normalWS : NORMAL;
    float3 positionWS : POSITION1;
};

cbuffer ObjectConstants : register(b0)
{
    float4x4 model;
    int id;
}

cbuffer FrameConstants : register(b1)
{
    float4x4 view;
    float4x4 projection;
    float3 cameraPosition;
}

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    float4 worldPos = mul(float4(input.position, 1.0), model);
    float4 viewPos = mul(worldPos, view);
    float4 projPos = mul(viewPos, projection);
    output.position = projPos;

    output.texcoord = input.texcoord;
    output.positionWS = worldPos.xyz;
    
    output.normalWS = normalize(mul(input.normal, (float3x3) model));
    output.tangentWS = normalize(mul(input.tangent, (float3x3) model));
    output.bitangentWS = normalize(cross(output.normalWS, output.tangentWS));

    return output;
}