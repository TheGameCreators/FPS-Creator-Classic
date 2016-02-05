//
// Cubemap Glass
//

// model info
string XFile = "sphere.x";

// textures
texture LMMap : LMMap
<
    string Name = "default_colorandalpha.dds";
    string type = "2D";
>;

texture DiffuseMap : DiffuseMap
<
    string Name = "default_colorandalpha.dds";
    string type = "2D";
>;

texture EnvironmentMap 
< 
    string type = "CUBE"; 
    string name = "cubemap.dds"; 
>;

// transformations
float4x3 WorldView  : WORLDVIEW;
float4x4 Projection : PROJECTION;

// light direction (view space)
float3 L < string UIDirectional = "Light Direction"; > = normalize(float3(-0.397f, -0.397f, 0.827f));

struct VS_OUTPUT
{
    float4 Position   : POSITION;
    float2 Tex : TEXCOORD0;               
    float2 Tex2 : TEXCOORD1;               
    float3 Reflection : TEXCOORD2;               
};

// vertex shader
VS_OUTPUT VS(    
    float3 Position : POSITION,
    float2 TexCoord : TEXCOORD0,
    float3 Normal   : NORMAL)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    L = -L;

    float3 P = mul(float4(Position, 1), (float4x3)WorldView);   // position (view space)
    float3 N = normalize(mul(Normal, (float3x3)WorldView));     // normal (view space)
    float3 V = -normalize(P);                                   // view direction (view space)
    float3 G = normalize(2 * dot(N, V) * N - V);                // glance vector (view space)

    // position (projected)
    Out.Position = mul(float4(P, 1), Projection);             
                                 
    // environment cube map coordinates
    Out.Tex = TexCoord;                   
    Out.Tex2 = TexCoord;                   
    Out.Reflection = float3(-G.x, G.y, -G.z);                   
            
    return Out;
}

// samplers
sampler LM = sampler_state
{ 
    Texture = (LMMap);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler Diffuse = sampler_state
{ 
    Texture = (DiffuseMap);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler Environment = sampler_state
{ 
    Texture = (EnvironmentMap);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// pixel shader
float4 PS(VS_OUTPUT In) : COLOR
{   
    float4 Color = (float4)0;
    float3 Env;

    // combine the contributions
    Env = texCUBE(Environment, In.Reflection);
    Color.rgb = tex2D(Diffuse, In.Tex2).xyz * Env;
    Color.w   = 0.2;

    return Color;
}  

// technique
technique TCubeMapGlass
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PS();

        CullMode         = NONE;

        // enable alpha blending
        AlphaBlendEnable = TRUE;
        SrcBlend         = SRCALPHA;
        DestBlend        = INVSRCALPHA;
    }
}

