//
// Bump + Diffuse + Specular (animated bump)
//

/************* UNTWEAKABLES **************/

float4x4 WorldIT : WorldInverseTranspose;
float4x4 WorldViewProj : WorldViewProjection;
float4x4 World : World;
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

float4 LightPos : Position
<
    string UIObject = "PointLight";
    string Space = "World";
> = {100.0f, 100.0f, -100.0f, 0.0f};

float4 FixedLightSource
<
    string UIType = "Fixed Light Source";
> = {0.0f, 50000.0f, 0.0f, 0.0f};

float4 LightColor
<
	string UIType = "Color";
> = {0.75f, 0.75f, 0.75f, 1.0f};

float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
> = {0.01f, 0.01f, 0.01f, 1.0f};

float4 SurfColor : Diffuse
<
    string UIName =  "Surface Color";
    string UIType = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float SpecExpon : Power
<
    string UIType = "slider";
    float UIMin = 1.0;
    float UIMax = 128.0;
    float UIStep = 1.0;
    string UIName =  "specular power";
> = 2.0;

float Bumpy
<
    string UIType = "slider";
    float UIMin = 0.0;
    float UIMax = 10.0;
    float UIStep = 0.1;
    string UIName =  "bump power";
> = -1.0;

float Alphavalue : AlphaOverride
<
    string UIName =  "Alpha Override";
> = 1.0f;

/************* TEXTURES **************/

texture colorTexture : DiffuseMap
<
    string Name = "default_color.dds";
    string type = "2D";
>;

texture normalTexture : DiffuseMap
<
    string Name = "default_bump_normal.dds";
    string type = "2D";
>;

texture reflectMap : ReflectMap
<
	string Name = "default_mask.dds";
    string type = "2D";
>;

sampler2D colorSampler = sampler_state
{
	Texture = <colorTexture>;
	MinFilter = Linear;
	MagFilter = ANISOTROPIC;
	MipFilter = Linear;
};

sampler2D normalSampler = sampler_state
{
	Texture = <normalTexture>;
	MinFilter = Linear;
	MagFilter = ANISOTROPIC;
	MipFilter = Linear;
};

sampler2D reflectSampler = sampler_state
{
	Texture = <reflectMap>;
	MinFilter = Linear;
	MagFilter = ANISOTROPIC;
	MipFilter = Linear;
};

/************* DATA STRUCTS **************/

struct appdata {
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
    float4 Blendweight	: TEXCOORD1;
    float4 Blendindices	: TEXCOORD2;
};

/* data passed from vertex shader to pixel shader */
struct vertexOutput {
    float4 HPosition	: POSITION;
    float4 TexCoord	: TEXCOORD0;
    float3 LightVec	: TEXCOORD1;
    float3 WorldNormal	: TEXCOORD2;
    float3 WorldEyeVec	: TEXCOORD3;
    float3 WorldTangent	: TEXCOORD4;
    float3 WorldBinorm	: TEXCOORD5;
    float clip : TEXCOORD6;
    float3 WPos : TEXCOORD7;
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

    
    float4 worldSpacePos = mul(tempPos, World);
    OUT.WPos = worldSpacePos;
    OUT.WorldNormal = mul(netNormal, WorldIT).xyz;
    OUT.WorldTangent = mul(IN.Tangent, WorldIT).xyz;
    OUT.WorldBinorm = mul(IN.Binormal, WorldIT).xyz;
    OUT.LightVec = FixedLightSource - worldSpacePos.xyz;
    OUT.TexCoord = IN.UV;
    OUT.WorldEyeVec = normalize(ViewInv[3].xyz - worldSpacePos.xyz);
    OUT.HPosition = mul(tempPos, WorldViewProj);

    // all shaders should send the clip value to the pixel shader (for refr/refl)                                                                     
    // OUT.clip = dot(worldSpacePos, clipPlane); // too expensive for VS2.0
    OUT.clip = (worldSpacePos.y * clipPlane.y) + clipPlane.a; // good for water plane

    return OUT;
}

/********* pixel shader ********/

float4 CalcSpotFlash( float3 worldNormal, float3 worldPos )
{
    float4 output = (float4)0.0;
    float3 toLight = SpotFlashPos.xyz - worldPos.xyz;
    float3 lightDir = normalize( toLight );
    float lightDist = length( toLight );
    
    float MinFalloff = 200;  //falloff start distance - 50,0,.01 are very cool too for lanterns
    float LinearFalloff = 1;
    float ExpFalloff = .005;  // 1/200
    
    float fAtten = 1.0/(MinFalloff + (LinearFalloff*lightDist)+(ExpFalloff*lightDist*lightDist));
    
    SpotFlashPos.w = clamp(0,1,SpotFlashPos.w -.2);
    
    
    output += max(0,dot( lightDir, worldNormal ) * 2.5*SpotFlashColor*fAtten * (SpotFlashPos.w) );
    
    return output;
}

float4 mainPS(vertexOutput IN) : COLOR
{
    // all shaders should receive the clip value                                                                
    clip(IN.clip);

    float4 map = tex2D(colorSampler,IN.TexCoord.xy);
    float3 bumps = Bumpy * (tex2D(normalSampler,IN.TexCoord.xy).xyz-(0.5).xxx);
    float3 Ln = normalize(IN.LightVec);
    float3 Nn = normalize(IN.WorldNormal);
    float3 Tn = normalize(IN.WorldTangent);
    float3 Bn = normalize(IN.WorldBinorm);
    float3 Nb = Nn + (bumps.x * Tn + bumps.y * Bn);
    Nb = normalize(Nb);
    float3 Vn = normalize(IN.WorldEyeVec);
    float3 Hn = normalize(Vn + Ln);
    float4 lighting = lit(dot(Ln,Nb),dot(Hn,Nb),SpecExpon);
    float hdn = lighting.z;
    float ldn = lighting.y;

    float diffComp = ldn;
    float4 spotflashlighting = CalcSpotFlash ( IN.WorldNormal, IN.WPos.xyz );
    float4 diffContrib2 = SurfColor * map * ldn;
    float4 diffContrib = diffContrib2 + (spotflashlighting+AmbiColor * map);
    float4 specContrib = SurfColor * map * hdn;
    float4 result = diffContrib + specContrib;
    result.w=map.w * Alphavalue;

    return result;
}

/****** technique *******/

technique dx9textured
{
	pass p0 
	{		
	        // lighting
        	Lighting       = FALSE;
        	SpecularEnable = FALSE;
	        FogEnable      = FALSE;

		// same vertex shader for both DX9 techniques
		VertexShader = compile vs_2_0 mainVS();
		ZEnable = true;
		ZWriteEnable = true;
		CullMode = None;
		PixelShader = compile ps_2_0 mainPS();
	}
}

/***************************** eof ***/
