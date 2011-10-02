//
// Fast Bone
//

/************* UNTWEAKABLES **************/

float4x4 WorldIT : WorldInverseTranspose;
float4x4 WorldViewProj : WorldViewProjection;
float4x4 World : World;
float4x4 View : View;
float4x4 ViewInv : ViewInverse;
float4 clipPlane : ClipPlane;

/*********** DBPRO UNTWEAKABLES **********/
float4x4 boneMatrix[60] : BoneMatrixPalette;

/*********** SPOTFLASH VALUES FROM FPSC **********/

float4 SpotFlashPos;


float4 SpotFlashColor;


float SpotFlashRange   //fixed value that FPSC uses
<
    string UIName =  "SpotFlash Range";
    
> = {600.00};

/************* SURFACE **************/

float4 lhtDir < string UIDirectional = "Light Direction"; >; 

float4 LightPos : Position
<
    string UIObject = "PointLight";
    string Space = "World";
> = {100.0f, 100.0f, -100.0f, 0.0f};

float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
> = {0.01f, 0.01f, 0.01f, 1.0f};

float4 SurfColor : Diffuse
<
    string UIName =  "Surface Color";
    string UIType = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float4 FogColor : Diffuse
<
    string UIName =  "Fog Color";
    string UIType = "Color";
> = {1.0f, 0.0f, 0.0f, 0.000001f};

float4 FillerColor : Diffuse
<
    string UIName =  "Filler Color";
    string UIType = "Color";
> = {0.0f, 0.0f, 0.0f, 0.0f};

/************* TEXTURES **************/

texture colorTexture : DiffuseMap
<
    string Name = "default_color.dds";
    string type = "2D";
>;

sampler2D colorSampler = sampler_state
{
	Texture = <colorTexture>;
	MinFilter = Linear;
	MagFilter = ANISOTROPIC;
	MipFilter = Linear;
};

/************* DATA STRUCTS **************/

struct appdata {
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    float4 Blendweight	: TEXCOORD1;
    float4 Blendindices	: TEXCOORD2;
};

/* data passed from vertex shader to pixel shader */
struct vertexOutput {
    float4 HPosition	: POSITION;
    float4 TexCoord	: TEXCOORD0;
    float3 Normal	: TEXCOORD1;
    float3 Fog		: TEXCOORD2;
    float clipvalue     : TEXCOORD3;
    float3 WPos : TEXCOORD4;
    
};

/*********** vertex shader ******/

vertexOutput mainVS(appdata IN)
{
    vertexOutput OUT;
    float3 netPosition = 0, netNormal = 0;
    for (int i = 0; i < 4; i++)
    {
     float index = IN.Blendindices[i];
     float3x4 model = float3x4(boneMatrix[index][0], boneMatrix[index][1], boneMatrix[index][2]);
     float3 vec3 = mul(model, float4(IN.Position, 1));
     vec3 = vec3 + boneMatrix[index][3].xyz;
     float3x3 rotate = float3x3(model[0].xyz, model[1].xyz, model[2].xyz); 
     float3 norm3 = mul(rotate, IN.Normal);
     netPosition += vec3.xyz * IN.Blendweight[i];
     netNormal += norm3.xyz * IN.Blendweight[i];
    }
    float4 tempPos = float4(netPosition,1.0);
    netNormal = normalize(netNormal);
    
    

    float3 worldSpacePos = mul(tempPos, World).xyz;
    OUT.WPos = worldSpacePos; ///spotmod
    OUT.TexCoord = IN.UV;
    OUT.HPosition = mul(tempPos, WorldViewProj);

//    float3 L = -lhtDir;
//    float4 gogo;
//    gogo = (max(0, dot(netNormal, L))*SurfColor*0.25) + (SurfColor*0.75) + AmbiColor;
//    gogo.w=1;
//    OUT.Col = gogo;
    //OUT.Normal = netNormal;
    OUT.Normal = mul(netNormal, WorldIT).xyz;

    float4 cameraPos = float4(1,1,1,1); //mul( float4(worldSpacePos,1), View );
    float fogstrength = cameraPos.z * FogColor.w;
    OUT.Fog = FogColor.xyz * fogstrength;

    // all shaders should send the clip value to the pixel shader (for refr/refl)                                                                     
    // OUT.clip = dot(worldSpacePos, clipPlane); // too expensive for VS2.0
    OUT.clipvalue = (worldSpacePos.y * clipPlane.y) + clipPlane.a; // good for water plane

    return OUT;
}

float4 CalcSpotFlash( float3 worldNormal, float3 worldPos )
{
    float4 output = (float4)0.0;
    float3 toLight = SpotFlashPos.xyz - worldPos.xyz;
    float3 lightDir = normalize( toLight );
    float lightDist = length( toLight );
    
    float MinFalloff = 200;  //minimum falloff distance
    float LinearFalloff = 1;
    float ExpFalloff = .005;  // 1/200
    
    float fAtten = 1.0/(MinFalloff + (LinearFalloff*lightDist)+(ExpFalloff*lightDist*lightDist));
    
    SpotFlashPos.w = clamp(0,1,SpotFlashPos.w -.2); 
    
    
    output += max(0,dot( lightDir, worldNormal ) * 2.0*SpotFlashColor*fAtten * (SpotFlashPos.w) );
    
    return output;
}

float4 PS(
    float2 Tex  : TEXCOORD0,
    float3 Normal : TEXCOORD1,	
    float3 Fog  : TEXCOORD2,
    float clipvalue  : TEXCOORD3, vertexOutput IN ) : COLOR
{
    clip(clipvalue);
    
    float4 spotflashlighting = CalcSpotFlash ( IN.Normal, IN.WPos.xyz );  //spotmod

    float3 L = -lhtDir;
    float4 gogo;
    gogo = (max(0, dot(Normal, L))*SurfColor*0.25) + (SurfColor*0.75) + AmbiColor +spotflashlighting;
    gogo.w=1;

    return (tex2D(colorSampler, Tex) * gogo) + FillerColor + float4(Fog,1.0) ;
}

/****** technique *******/

technique dx9textured
{
	pass p0 
	{		
	        Sampler[0] = (colorSampler);
		VertexShader = compile vs_2_0 mainVS();
	        PixelShader  = compile ps_2_0 PS();
	}
}
