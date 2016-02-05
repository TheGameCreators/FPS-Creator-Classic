// Fake Bump (uses a normal map to extract a specular map)

/************* UNTWEAKABLES **************/

float4x4 World      : WORLD;

float4x4 WorldViewProj : WorldViewProjection;
float4x4 WorldView : WorldView;
float4x4 WorldIT : WorldInverseTranspose;
float4x4 ViewInv : ViewInverse;
float4 eyePos : CameraPosition;
float time : Time;
float4 clipPlane : ClipPlane;
float4x4 View : View;

/******TWEAKABLES***************************/

float depthScale
<
	string UIWidget = "slider";
	float UIMax = 0.1;
	float UIMin = 0.001;
	float UIStep = 0.001;
> = 0.015;

/******VALUES PULLED FROM FPSC - NON TWEAKABLE**********/

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


/****************** TEXTURES AND SAMPLERS*********************/

texture LightMap : DiffuseMap
<
    string Name = "LM.tga";
    string type = "2D";
>;

texture DiffuseMap : DiffuseMap
<
    string Name = "D.tga";
    string type = "2D";
>;

texture EffectMap : DiffuseMap
<
    string Name = "I.tga";
    string type = "2D";
>;

//Lightmap texture
sampler2D LightmapSampler = sampler_state
{
    Texture   = <LightMap>;
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

//Diffuse Texture _D
sampler2D DiffuseSampler = sampler_state
{
    Texture   = <DiffuseMap>;
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

//Effect Texture _I (specular texture)
sampler2D EffectSampler = sampler_state
{
    Texture   = <EffectMap>;
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

/************* DATA STRUCTS **************/

struct appdata {
    float4 Position	: POSITION;
    float4 UV0		: TEXCOORD0;
    float4 UV1		: TEXCOORD1;
    float4 Normal	: NORMAL;
    float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
};

/*data passed to pixel shader*/
struct vertexOutput
{
    float4 Position    : POSITION;
    float2 TexCoord     : TEXCOORD0;
    float2 TexCoordLM   : TEXCOORD1;
    float3 LightVec	    : TEXCOORD2;
    float3 WorldNormal	: TEXCOORD3;
    float4 WPos : TEXCOORD4;
    float4 ppos : TEXCOORD5;
    float Fog : TEXCOORD6;  
    float clip : TEXCOORD7;
};


/*******Vertex Shader***************************/

vertexOutput mainVS(appdata IN)   
{
    vertexOutput OUT;   
    float4 worldSpacePos = mul(IN.Position, World);
    OUT.WorldNormal = normalize(mul(IN.Normal, WorldIT).xyz);
    OUT.LightVec = normalize (eyePos+25 - worldSpacePos );  //adding in a slight offset to eyePos for some variation to spec position
    OUT.Position = mul(IN.Position, WorldViewProj);
    OUT.TexCoord  = IN.UV0; 
    OUT.TexCoordLM  = IN.UV1; 
    OUT.WPos =   worldSpacePos;                                                                                
    OUT.ppos = mul( IN.Position, WorldView );      

    // calculate FOG colour
    float4 cameraPos = mul( worldSpacePos, View );
    float fogstrength = cameraPos.z * FogColor.w;
    OUT.Fog = min(fogstrength,1.0);
                     
    // all shaders should send the clip value to the pixel shader (for refr/refl)
    OUT.clip = dot(worldSpacePos, clipPlane);

    return OUT;
}

/****************Framgent Shader*****************/

float4 CalcLighting( float3 worldNormal, float3 worldPos )
{
    float4 output = (float4)0.0;
    // light 0
    float3 toLight = g_lights_pos0.xyz - worldPos;
    float lightDist = length( toLight );
    float fAtten = 1.0/dot( g_lights_atten0, float4(1,lightDist,lightDist*lightDist,0) );
    float3 lightDir = normalize( toLight );
    output += max(0,dot( lightDir, worldNormal ) * g_lights_diffuse0 * fAtten);
 //   // light 1
 //   toLight = g_lights_pos1.xyz - worldPos;
 //   lightDist = length( toLight );
 //   fAtten = 1.0/dot( g_lights_atten1, float4(1,lightDist,lightDist*lightDist,0) );
 //   lightDir = normalize( toLight );
 //   output += max(0,dot( lightDir, worldNormal ) * g_lights_diffuse1 * fAtten);

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

float4 mainPS(vertexOutput IN) : COLOR
{
    // all shaders should receive the clip value                                                                
    clip(IN.clip);
    float4 LM = tex2D(LightmapSampler,IN.TexCoordLM);         //sample lightmap texture
    float4 diffuse = tex2D(DiffuseSampler,IN.TexCoord.xy);    //sample diffuse texture    
    float4 effectmap = tex2D(EffectSampler,IN.TexCoord.xy);   //sample specular map texture 
    float3 Ln = (IN.LightVec);
    float3 Nn = normalize(IN.WorldNormal);
    float3 V  = normalize(eyePos - IN.WPos);                  //create normalized view vector for constant forward "hero" spec
    float3 Hn = normalize(V+Ln);                              //half vector
    float dis = distance(IN.WPos,eyePos);
    float atten = (1/(dis*(dis*.01)))* 2000 ;                 //last value is multiplier, inverse square faloff
    atten = clamp(atten,0,1.5);                               //second value controls how bright to let the highlights become
    float herospec = pow(max(dot(Nn,Hn),0),10);               //specular highlights 
    float4 fakespecmap = float4((effectmap.z-((abs(effectmap.x-0.5)+abs(effectmap.y-0.5))*3)).xxx,1);
    float4 specular = (herospec)*(fakespecmap*LM*3)*diffuse*atten; //multiply spec texture, lightmap, and diffuse texture
    float4 dynamiclighting = CalcLighting ( IN.WorldNormal, IN.WPos.xyz );
    float4 LMfinal = (LM + AmbiColor + dynamiclighting)*diffuse;
    float4 result =   LMfinal + specular;
    return float4((result.xyz*(1-IN.Fog))+(FogColor.xyz*IN.Fog),diffuse.w);
}

technique alpha
{
    pass P0
    {
        Lighting       = FALSE;
        FogEnable      = FALSE;
        VertexShader = compile vs_2_0 mainVS();
        PixelShader  = compile ps_2_0 mainPS();
    }
}
