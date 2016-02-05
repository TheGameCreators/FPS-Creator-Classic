//
// Ripple
//

matrix worldi : WorldIT;
matrix wvp : WorldViewProjection;
matrix mworld : World;
matrix viewInv : ViewIT;
float4 lightPos : Position < string UIPosition = "Light Position"; >;
float ticks : Time;

string XFile = "water.x";
texture LMTexture < string Name = "blue.tga"; >;
texture colorTexture < string Name = "blue.tga"; >;

struct VS_INPUT
{
    float3 Position     : POSITION;
    float4 UV           : TEXCOORD0;
    float3 Normal       : NORMAL;
};

struct VS_OUTPUT
{
    float4 HPosition    : POSITION;
    float4 TexCoord0    : TEXCOORD0;
    float4 diffCol      : COLOR;
};

float4 ambiColor={0.2f, 0.2f, 0.2f, 1.0f};
float specExpon=22.0;
float horizontal=2.0;
float vertical=3.0;
float timeScale=0.005;

VS_OUTPUT VShade(VS_INPUT IN)
{
    VS_OUTPUT   OUT;

    // world normal
    float3 worldNormal = mul(IN.Normal, mworld).xyz;
    worldNormal = normalize(worldNormal);
    float timeNow = ((ticks/100.0)/timeScale);
    float4 tempPos;
    tempPos.xyz = IN.Position.xyz;
    tempPos.w   = 1.0;
    OUT.HPosition = mul(tempPos, wvp);

    float iny = tempPos.x * vertical + timeNow;
    float wiggleX = sin(iny) * horizontal;
    float wiggleY = cos(iny) * horizontal; // deriv
    worldNormal.y = worldNormal.y + wiggleY;
    worldNormal = normalize(worldNormal);
    tempPos.x = tempPos.x + wiggleX;
    tempPos.y = tempPos.y + wiggleY;

    float4 diffContrib = float4(1,1,1,1) + ambiColor/2;
    OUT.diffCol = diffContrib;
    OUT.TexCoord0.x = IN.UV.x + sin(timeNow)*0.1;
    OUT.TexCoord0.y = IN.UV.y + cos(timeNow)*0.1;
    OUT.TexCoord0.z = IN.UV.z;
    OUT.TexCoord0.w = IN.UV.w;

    return OUT;
}

uniform sampler   sampler0 = 
sampler_state 
{
    texture = <LMTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
};

uniform sampler   sampler1 = 
sampler_state 
{
    texture = <colorTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
};

struct PS_INPUT
{
    float4 TexCoord0    : TEXCOORD0;
    float4 diffCol      : COLOR;
};

float4 PShade(
    float4 Diff : COLOR0,
    float2 Tex  : TEXCOORD0) : COLOR
{
    return tex2D(sampler1, Tex) * Diff;
}

//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique t0
{
    pass p0
    {
        VertexShader = compile vs_2_0 VShade();
        PixelShader = compile ps_1_0 PShade();

        // enable alpha blending
        FogEnable    = FALSE;
        AlphaBlendEnable = TRUE;
        SrcBlend         = SRCALPHA;
        DestBlend        = INVSRCALPHA;

        MinFilter[0] = Linear;
        MagFilter[0] = Linear;
        MipFilter[0] = Linear;
    }
}
