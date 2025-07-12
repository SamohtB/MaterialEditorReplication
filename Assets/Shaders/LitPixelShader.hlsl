Texture2D Textures[] : register(t0, space0);
SamplerState Samplers[] : register(s0, space0);

struct PSINPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 tangentWS : TANGENT;
    float3 bitangentWS : BITANGENT;
    float3 normalWS : NORMAL;
    float3 positionWS : POSITION1;
    float3 cameraPosition : CAMERA_POSITION;
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
    float smoothStr;
    float aoStr;

    float emmissiveStr;
    float heightStr;
    
    float2 tiling;
    float2 offset;
    
    float2 pad;
};

struct PointLightData
{
    float3 position;
    float range;

    float3 color;
    float intensity;
};

cbuffer LightConstants : register(b3)
{
    PointLightData pointLights[4];
    uint pointLightCount;
    float3 padding;
};

static const uint HasAlbedoMap = 1 << 0;
static const uint HasNormalMap = 1 << 1;
static const uint HasMetallicMap = 1 << 2;
static const uint HasRoughnessMap = 1 << 3;
static const uint HasAOMap = 1 << 4;
static const uint HasEmissiveMap = 1 << 5;
static const uint HasHeightMap = 1 << 6;

struct SampledTextureMaps
{
    float3 albedo;
    float3 normal;
    float3 MRAO;
};

static const float PI = 3.14159265359;

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

SampledTextureMaps SampleTextures(PSINPUT input, float3x3 TBN, float3 V)
{
    // === Default Values ===
    float3 d_normal = float3(0, 0, 1); 
    float d_metallic = 0.0f;
    float d_smoothness = 0.0f;
    float d_ao = 1.0f;
    
    float2 parallaxUV = input.texcoord * tiling + offset;
    
    SampledTextureMaps samples;
    
    // === Check Flags ===
    
    // Check Height Map first to determine if we need to apply parallax mapping
    // === Height Map ===
    if ((materialFlags & HasHeightMap) != 0 && heightStr > 0.0f)
    {
        float3 viewDirTS = normalize(mul(TBN, V));   
        float height = Textures[heightHandleIndex].Sample(Samplers[0], input.texcoord).r;

        float parallaxScale = 0.04f;
        float parallaxBias = 0.01f;
        
        float2 offset = viewDirTS.xy * (height * parallaxScale - parallaxBias) * heightStr;
        parallaxUV += offset;
    }
    
    // === Albedo Map ===
    if ((materialFlags & HasAlbedoMap) != 0)
    {
        float3 albedoTex = Textures[diffuseHandleIndex].
        Sample(Samplers[0], parallaxUV).rgb;
        samples.albedo = pow(albedoTex, 2.2f) * baseColor.rgb;
    }
    else
    {
        samples.albedo = baseColor.rgb;
    }
    
    // === Normal Map ===
    if ((materialFlags & HasNormalMap) != 0 && normalStr > 0.0f)
    {
        float3 sampledNormal = Textures[normalHandleIndex].Sample(Samplers[0], parallaxUV).rgb;
        sampledNormal = normalize(sampledNormal * 2.0 - 1.0);
        samples.normal = normalize(lerp(d_normal, sampledNormal, normalStr));
    }
    else
    {
        samples.normal = input.normalWS;
    }
    
    // === Metallic ===
    if ((materialFlags & HasMetallicMap) != 0 && metalStr > 0.0f)
    {
        float metal = Textures[metalHandleIndex].Sample(Samplers[0], parallaxUV).r;
        samples.MRAO.r = lerp(d_metallic, metal, metalStr);
    }
    else
    {
        samples.MRAO.r = d_metallic;
    }
    
    // === Smoothness ===
    if ((materialFlags & HasRoughnessMap) != 0 && smoothStr > 0.0f)
    {
        float smooth = 1 - Textures[roughHandleIndex].Sample(Samplers[0], parallaxUV).r;
        samples.MRAO.g = lerp(d_smoothness, smooth, smoothStr);
    }
    else
    {
        samples.MRAO.g = d_smoothness;
    }
    
    // === AO Map ===
    if ((materialFlags & HasAOMap) != 0 && aoStr > 0.0f)
    {
        float ao = Textures[aoHandleIndex].Sample(Samplers[0], parallaxUV).r;
        samples.MRAO.b = lerp(d_ao, ao, aoStr);
    }
    
    else
    {
        samples.MRAO.b = d_ao;
    }
    
    return samples;
}

float4 PSMain(PSINPUT input) : SV_TARGET
{
    
    float3x3 TBN = float3x3(input.tangentWS, input.bitangentWS, input.normalWS);
    float3 V = input.cameraPosition - input.positionWS;
    
    SampledTextureMaps samples = SampleTextures(input, TBN, V);
    
    V = normalize(V);
    float3 N = normalize(mul(TBN, normalize(samples.normal * 2.0 - 1.0)));
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), samples.albedo, samples.MRAO.r);
    float3 Lo = float3(0.0, 0.0, 0.0);
    
    for (uint i = 0; i < pointLightCount; ++i)
    {
        PointLightData light = pointLights[i];
        float3 L = light.position - input.positionWS;
        float distance = length(L);
        L = L / distance;

        float3 H = normalize(V + L);
        float attenuation = saturate(1.0 - distance / light.range);
        attenuation *= attenuation; // optional quadratic falloff

        float3 radiance = light.color * light.intensity * attenuation;

        float NDF = DistributionGGX(N, H, samples.MRAO.g);
        float G = GeometrySmith(N, V, L, samples.MRAO.g);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        float3 kS = F;
        float3 kD = (1.0 - kS) * (1.0 - samples.MRAO.r);

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        float3 specular = numerator / denominator;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * samples.albedo / PI + specular) * radiance * NdotL;
    }

    float3 ambient = float3(0.03, 0.03, 0.03) * samples.albedo * samples.MRAO.b;
    float3 color = ambient + Lo;

    color = color / (color + 1.0);
    color = pow(color, 1.0 / 2.2);
    
    return float4(color, 1.0);
}