//
// Illumination Map
//

string XFile = "sphere.x";

float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
> = {0.1f, 0.1f, 0.1f, 1.0f};

float4 SurfColor : Diffuse
<
    string UIName =  "Surface Color";
    string UIType = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float depthScale
<
	string UIWidget = "slider";
	float UIMax = 0.1;
	float UIMin = 0.001;
	float UIStep = 0.001;
> = 0.015;

// texture
texture Tex1 < string name = "D.tga"; >;
texture Tex2 < string name = "I.tga"; >;

// transformations
float4x4 World      : WORLD;
float4x4 WorldView  : WorldView;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float2 Tex1 : TEXCOORD0;
    float2 Tex2 : TEXCOORD1;
    float4 ppos : TEXCOORD2;
};

VS_OUTPUT VS(
    float4 Pos  : POSITION, 
    float3 Norm : NORMAL, 
    float2 Tex1 : TEXCOORD0)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float4x4 WorldView = mul(World, View);
    float3 P = mul(Pos, (float4x3)WorldView);  // position (view space)
    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
    Out.Diff = SurfColor + AmbiColor;
    Out.Diff.w = 1.0f;
    Out.Tex1 = Tex1;                                       
    Out.Tex2 = Tex1;   
    Out.ppos = mul( Pos, WorldView );

    return Out;
}

sampler Sampler1 = sampler_state
{
    Texture   = (Tex1);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler Sampler2 = sampler_state
{
    Texture   = (Tex2);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4 PS(
    float4 Diff  : COLOR0,
    float2 Tex   : TEXCOORD0,
    float2 TexI  : TEXCOORD1,
    float4 ppos  : TEXCOORD2) : COLOR
{
    return (Diff+tex2D(Sampler2, TexI)) * tex2D(Sampler1, Tex);
}
float4 PSdepth(
    float4 Diff  : COLOR0,
    float2 Tex   : TEXCOORD0,
    float2 TexI  : TEXCOORD1,
    float4 ppos  : TEXCOORD2) : COLOR
{
    float4 result = (Diff+tex2D(Sampler2, TexI)) * tex2D(Sampler1, Tex);
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
        Sampler[0] = (Sampler1);
        Sampler[1] = (Sampler2);
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
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = FALSE;
        Sampler[0] = (Sampler1);
        Sampler[1] = (Sampler2);
        VertexShader = compile vs_1_0 VS();
        PixelShader  = compile ps_2_0 PSdepth();
    }
}
