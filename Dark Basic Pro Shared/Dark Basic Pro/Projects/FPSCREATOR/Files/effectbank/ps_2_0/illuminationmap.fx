//
// Illumination Map
//

string XFile = "sphere.x";   // model
int    BCLR = 0xff202080;   // background

// light direction (view space)
float3 lightDir <  string UIDirectional = "Light Direction"; > = {0.577, -0.577, 0.577};
float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
> = {0.1f, 0.1f, 0.1f, 1.0f};

// Supports dynamic lights (using CalcLighting function)
float4 g_lights_pos0;
float4 g_lights_pos1;
float4 g_lights_pos2;
float4 g_lights_pos3;
float4 g_lights_atten0;
float4 g_lights_atten1;
float4 g_lights_atten2;
float4 g_lights_atten3;
float4 g_lights_diffuse0;
float4 g_lights_diffuse1;
float4 g_lights_diffuse2;
float4 g_lights_diffuse3;

//----------------
// FOG sensitivity
//----------------
float4 FogColor : Diffuse
<
    string UIName =  "Fog Color";
    string UIType = "Color";
> = {0.0f, 0.0f, 0.0f, 0.0000001f};

// material reflectivity
float4 k_d : MATERIALDIFFUSE = { 1.0f, 1.0f, 1.0f, 1.0f };    // diffuse
float4 k_s : MATERIALSPECULAR= { 0.0f, 0.0f, 0.0f, 0.0f };    // specular
int    n   : MATERIALPOWER = 32;                            // power

// texture
texture Tex0 < string name = "lm.tga"; >;
texture Tex1 < string name = "base.tga"; >;
texture Tex2 < string name = "illumination.tga"; >;

// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float4 clipPlane : ClipPlane;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float2 Tex  : TEXCOORD0;
    float2 Tex2  : TEXCOORD1;
    float2 Tex3  : TEXCOORD2;
    float3 WorldNormal : TEXCOORD3;
    float4 WPos : TEXCOORD4;
    float Fog : TEXCOORD5;  
    float clip : TEXCOORD6;
};

VS_OUTPUT VS(
    float3 Pos  : POSITION, 
    float3 Norm : NORMAL, 
    float2 Tex  : TEXCOORD0, 
    float2 Tex2 : TEXCOORD1)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float4x4 WorldView = mul(World, View);
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);  // position (view space)
    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
    Out.Diff = AmbiColor; 		  // ambient
    Out.Diff.w = 1.0f;
    Out.Tex  = Tex;                                       
    Out.Tex2  = Tex;                                       
    Out.Tex3  = Tex2;

    // calculate FOG colour
    float4 worldSpacePos = mul(float4(Pos,1), World);                                              Out.WorldNormal = mul(float4(Norm,1), World);        
    Out.WPos = worldSpacePos;

    float4 cameraPos = mul( worldSpacePos, View );
    float fogstrength = cameraPos.z * FogColor.w;
    Out.Fog = min(fogstrength,1.0);

    // all shaders should send the clip value to the pixel shader (for refr/refl)
    Out.clip = dot(worldSpacePos, clipPlane);

    return Out;
}

sampler Sampler = sampler_state
{
    Texture   = (Tex0);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

sampler Sampler2 = sampler_state
{
    Texture   = (Tex1);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

sampler Sampler3 = sampler_state
{
    Texture   = (Tex2);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

float4 CalcLighting( float3 worldNormal, float3 worldPos )
{
    float4 output = (float4)0.0;
    // light 0
    float3 toLight = g_lights_pos0.xyz - worldPos;
    float lightDist = length( toLight );
    float fAtten = 1.0/dot( g_lights_atten0, float4(1,lightDist,lightDist*lightDist,0) );
    float3 lightDir = normalize( toLight );
    output += max(0,dot( lightDir, worldNormal ) * g_lights_diffuse0 * fAtten);
    // light 1
    toLight = g_lights_pos1.xyz - worldPos;
    lightDist = length( toLight );
    fAtten = 1.0/dot( g_lights_atten1, float4(1,lightDist,lightDist*lightDist,0) );
    lightDir = normalize( toLight );
    output += max(0,dot( lightDir, worldNormal ) * g_lights_diffuse1 * fAtten);

// (pixel shader 2.0 runs out of space here)
//    // light 2 
//    toLight = g_lights_pos2.xyz - worldPos;
//    lightDist = length( toLight );
//    fAtten = 1.0/dot( g_lights_atten2, float4(1,lightDist,lightDist*lightDist,0) );
//    lightDir = normalize( toLight );
//    output += max(0,dot( lightDir, worldNormal ) * g_lights_diffuse2 * fAtten);
//    // light 3
//    toLight = g_lights_pos3.xyz - worldPos;
//    lightDist = length( toLight );
//    fAtten = 1.0/dot( g_lights_atten3, float4(1,lightDist,lightDist*lightDist,0) );
//    lightDir = normalize( toLight );
//    output += max(0,dot( lightDir, worldNormal ) * g_lights_diffuse3 * fAtten);

    return output;
}

float4 PS(VS_OUTPUT IN) : COLOR
{
    clip(IN.clip);
    float4 dynamiclighting = CalcLighting ( IN.WorldNormal, IN.WPos.xyz );
    float4 diffuse = tex2D(Sampler2, IN.Tex);
    float4 result = (tex2D(Sampler, IN.Tex3)+IN.Diff+dynamiclighting+tex2D(Sampler3, IN.Tex2)) * diffuse;
    return float4((result.xyz*(1-IN.Fog))+(FogColor.xyz*IN.Fog),diffuse.w);
}

technique TVertexShaderOnly
{
    pass P0
    {
        // lighting
        Lighting       = FALSE;
        FogEnable      = FALSE;

        // samplers
        Sampler[0] = (Sampler);
        Sampler[1] = (Sampler2);
        Sampler[2] = (Sampler3);

        // shaders
        VertexShader = compile vs_1_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
