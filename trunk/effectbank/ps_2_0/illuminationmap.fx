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

float depthScale
<
	string UIWidget = "slider";
	float UIMax = 0.1;
	float UIMin = 0.001;
	float UIStep = 0.001;
> = 0.015;

// light intensity
float4 I_a = { 0.1f, 0.1f, 0.1f, 1.0f };    // ambient
float4 I_d = { 1.0f, 1.0f, 1.0f, 1.0f };    // diffuse
float4 I_s = { 1.0f, 1.0f, 1.0f, 1.0f };    // specular

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
float4x4 WorldView  : WorldView;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float2 Tex  : TEXCOORD0;
    float2 Tex2 : TEXCOORD1;
    float2 Tex3 : TEXCOORD2;
    float4 ppos : TEXCOORD3;
};

VS_OUTPUT VS(
    float4 Pos  : POSITION, 
    float3 Norm : NORMAL, 
    float2 Tex  : TEXCOORD0, 
    float2 Tex2 : TEXCOORD1)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float4x4 WorldView = mul(World, View);
    float3 P = mul(Pos, (float4x3)WorldView);  // position (view space)
    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
    Out.Diff = AmbiColor; 		  // ambient
    Out.Diff.w = 1.0f;
    Out.Tex  = Tex;                                       
    Out.Tex2  = Tex;                                       
    Out.Tex3  = Tex2;                                       
    Out.ppos = mul( Pos, WorldView );

    return Out;
}

sampler Sampler = sampler_state
{
    Texture   = (Tex0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler Sampler2 = sampler_state
{
    Texture   = (Tex1);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler Sampler3 = sampler_state
{
    Texture   = (Tex2);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4 PS(
    float4 Diff : COLOR0,
    float2 Tex  : TEXCOORD0,
    float2 Tex2  : TEXCOORD1,
    float2 Tex3  : TEXCOORD2,
    float4 ppos  : TEXCOORD3) : COLOR
{
    return (tex2D(Sampler, Tex3)+Diff+tex2D(Sampler3, Tex2)) * tex2D(Sampler2, Tex);
}
float4 PSdepth(
    float4 Diff : COLOR0,
    float2 Tex  : TEXCOORD0,
    float2 Tex2  : TEXCOORD1,
    float2 Tex3  : TEXCOORD2,
    float4 ppos  : TEXCOORD3) : COLOR
{
    float4 result = (tex2D(Sampler, Tex3)+Diff+tex2D(Sampler3, Tex2)) * tex2D(Sampler2, Tex);
    float a_depth = 1.0 - ( (depthScale/10.0f) * (ppos.z / ppos.w) );
    result.w = a_depth / 2.0f;
    return result;
}

technique alpha
{
    pass P0
    {
        Lighting       = FALSE;
        FogEnable      = FALSE;
        Sampler[0] = (Sampler);
        Sampler[1] = (Sampler2);
        Sampler[2] = (Sampler3);
        VertexShader = compile vs_1_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
technique depth
{
    pass P0
    {
        Lighting       = FALSE;
        FogEnable      = FALSE;
        Sampler[0] = (Sampler);
        Sampler[1] = (Sampler2);
        Sampler[2] = (Sampler3);
        VertexShader = compile vs_1_0 VS();
        PixelShader  = compile ps_2_0 PSdepth();
    }
}
