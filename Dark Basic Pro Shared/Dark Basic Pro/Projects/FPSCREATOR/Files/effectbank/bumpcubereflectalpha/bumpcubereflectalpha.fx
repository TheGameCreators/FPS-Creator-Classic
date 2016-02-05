//
// Bump + Diffuse + ( Cube * Reflect )
//

/************* UNTWEAKABLES **************/

float4x4 WorldIT : WorldInverseTranspose;
float4x4 WorldViewProj : WorldViewProjection;
float4x4 World : World;
float4x4 ViewInv : ViewInverse;

/************* SURFACE **************/

float4 LightSource
<
    string UIType = "Light Source";
> = {100.0f, 100.0f, -100.0f, 0.0f};

float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
> = {0.11f, 0.11f, 0.11f, 1.0f};

float4 SurfColor : Diffuse
<
    string UIName =  "Surface Color";
    string UIType = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float Bumpy
<
    string UIType = "slider";
    float UIMin = 0.0;
    float UIMax = 10.0;
    float UIStep = 0.1;
    string UIName =  "bump power";
> = 2.0;

/************* TEXTURES **************/

texture colorTexture : DiffuseMap
<
    string Name = "default_colorandalpha.dds";
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

texture cubeMap : EnvMap
<
	string Name = "default_cube.dds";
	string type = "Cube";
>;

sampler2D colorSampler = sampler_state
{
	Texture = <colorTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler2D normalSampler = sampler_state
{
	Texture = <normalTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler2D reflectSampler = sampler_state
{
	Texture = <reflectMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

samplerCUBE envMapSampler = sampler_state
{
	Texture = <cubeMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

/************* DATA STRUCTS **************/

/* data from application vertex buffer */
struct appdata {
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
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
};

/*********** vertex shader ******/

vertexOutput mainVS(appdata IN)
{
    vertexOutput OUT;
    OUT.WorldNormal = mul(IN.Normal, WorldIT).xyz;
    OUT.WorldTangent = mul(IN.Tangent, WorldIT).xyz;
    OUT.WorldBinorm = mul(IN.Binormal, WorldIT).xyz;
    float4 tempPos = float4(IN.Position.x,IN.Position.y,IN.Position.z,1.0);
    float3 worldSpacePos = mul(tempPos, World).xyz;
    OUT.LightVec = LightSource - worldSpacePos;
    OUT.TexCoord = IN.UV;
    OUT.WorldEyeVec = normalize(ViewInv[3].xyz - worldSpacePos);
    OUT.HPosition = mul(tempPos, WorldViewProj);
    return OUT;
}

/********* pixel shader ********/

float4 mainPS(vertexOutput IN) : COLOR
{
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
    float4 lighting = lit(dot(Ln,Nb),dot(Hn,Nb),12.0);
    float hdn = lighting.z;
    float ldn = lighting.y;

    float4 diffContrib2 = SurfColor * map;
    float4 diffContrib = diffContrib2 + (AmbiColor * map);
    float4 specContrib = (SurfColor*2) * map * hdn;
    float3 reflVect = reflect(Vn,Nn);
    float4 reflColor = texCUBE(envMapSampler,reflVect) * tex2D(reflectSampler,IN.TexCoord.xy) * diffContrib;
    float4 result = diffContrib + specContrib + reflColor;
    result.w=map.w; //  makde rockets disappear - possible full ghosted

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
