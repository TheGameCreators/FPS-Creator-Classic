/*********************************************************************
CHARACTER SHADER FOR SKINNED MESHES - Diffuse + Normal + Specular


Shader for skinned, bone-weighted models (characters).  Uses diffuse/normal/specular textures.
Pulls lighting info from nearest local light source.  Includes a second low-level hero light used
as a "fill" light.

It uses 3 textures:
texturename_D :diffuse texture, alpha channel controls transparency
texturename_N :normal map texture
texturename_S :specular level and color

****************************************************************************/

/************* UNTWEAKABLES **************/

float4x4 WorldIT : WorldInverseTranspose;
float4x4 WorldViewProj : WorldViewProjection;
float4x4 World : World;
float4x4 ViewInv : ViewInverse;
float4 clipPlane : ClipPlane;


/*********** DBPRO UNTWEAKABLES **********/
float4x4 boneMatrix[60] : BoneMatrixPalette;


/******SKY COLORS AND AMBIENT LIGHTING*******************/

//this value is pulled from FPSC - non tweakable
float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
    
> = { 0.010000, 0.010000, 0.010000, 1.000000 };

//this value is also pulled from fpsc - used with Ambicolor to calculate light decay
float4 SurfColor : Diffuse
<
    string UIName =  "Surface Color";
    string UIType = "Color";
> = { 1.000000, 1.000000, 1.000000, 1.000000 };





/************* LIGHTS **************/

float4 LightPos : Position
<
    string UIObject = "PointLight";
    string Space = "World";
    string UIWidget = "none";
> = { -100.000000, 700.000000, -200.000000, 1.000000 };


float4 LightSource
<
    string UIType = "Fixed Light Source";
> = { 400.000000, 300.000000, 600.000000, 1.000000 };



/*******SURFACE**************************/



float Bumpiness 
<
    string UIWidget = "slider";
    float UIMin = -5.0;
    float UIMax = 5.0;
    float UIStep = 0.1;
    string UIName =  "bumpiness";
> = 1.000000;

float Glossiness : Power
<
    string UIWidget = "slider";
    float UIMin = 1.0;
    float UIMax = 128.0;
    float UIStep = 1.0;
    string UIName =  "specular power";
> = 32.000000;



float Alphavalue : AlphaOverride
<
    string UIName =  "Alpha Override";
> = 1.0f;

/************* TEXTURES **************/

texture colorTexture : DiffuseMap
<
    string Name = "default_color.dds";
    string type = "2D";
 string ResourceName = "dante_D.dds"; >;

texture normalTexture : DiffuseMap
<
    string Name = "default_bump_normal.dds";
    string type = "2D";
 string ResourceName = "dante_N.dds"; >;

texture reflectMap : ReflectMap
<
	string Name = "default_mask.dds";
    string type = "2D";
 string ResourceName = "dante_S.dds"; >;

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
    float3 Wpos : TEXCOORD6;
    float clip : TEXCOORD7;
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
    OUT.WorldNormal = mul(netNormal, WorldIT).xyz;
    OUT.WorldTangent = mul(IN.Tangent, WorldIT).xyz;
    OUT.WorldBinorm = mul(IN.Binormal, WorldIT).xyz;
    OUT.LightVec = LightSource - worldSpacePos;
    OUT.TexCoord = IN.UV;
    OUT.WorldEyeVec = (ViewInv[3].xyz - worldSpacePos);
    OUT.HPosition = mul(tempPos, WorldViewProj);
    OUT.Wpos = worldSpacePos;

    // all shaders should send the clip value to the pixel shader (for refr/refl)                                                                     
    // OUT.clip = dot(worldSpacePos, clipPlane); // too expensive for VS2.0
    OUT.clip = (worldSpacePos.y * clipPlane.y) + clipPlane.a; // good for water plane

    return OUT;
}

/********* pixel shader ********/

float4 mainPS(vertexOutput IN) : COLOR
{
    // all shaders should receive the clip value                                                                
    clip(IN.clip);

    //sample textures
    float4 diffusemap = tex2D(colorSampler,IN.TexCoord.xy);
    float4 specmap = tex2D(reflectSampler,IN.TexCoord.xy);
    float3 normalmap = tex2D(normalSampler,IN.TexCoord.xy) * 2 - 1;
    
    float3 Ln = normalize(IN.LightVec);
    float3 Nn = (IN.WorldNormal);
    float3 Tn = (IN.WorldTangent);
    float3 Bn = (IN.WorldBinorm);
    float3 Nb = (normalmap.z * Nn) + (1.2 * (normalmap.x * Tn + normalmap.y * Bn));
    Nb = normalize(Nb);
    float3 Vn = normalize(IN.WorldEyeVec);
    float3 Hn = normalize(Vn + Ln);
    
            
    //calculate scene lighting Half Lambert scale and bias 
    float4 lighting = lit(pow(0.5*(dot(Ln,Nb))+0.5,2),dot(Hn,Nb),24);
   //float4 lighting = lit((dot(Ln,Nb)),dot(Hn,Nb),24);
    
    //hero lighting
    float3 offset = (0,500,0);
    float3 herolight = normalize( IN.WorldEyeVec+offset);
    float3 Hn2 = normalize(Vn + herolight);
    float4 herolighting = lit((dot(herolight,Nb)),dot(Hn2,Nb),24);
    

    
    //SurfColor = clamp(SurfColor,0.1,0.9);    
    //AmbiColor = clamp(AmbiColor,0,0.8);
    
    
    float4 diffContrib = SurfColor * diffusemap * (lighting.y) ;
    float4 specContrib = SurfColor *  specmap * lighting.z;
     
    float4 herodiffContrib =  diffusemap * (herolighting.y) ;
    float4 herospecContrib =   specmap  *  (herolighting.z);
    float4 herolightfinal = (herodiffContrib +herospecContrib );
    
    float4 ambContrib =  ((0.5*AmbiColor *  diffusemap)) + (0.5*AmbiColor*herolightfinal);	
 
    
    float4 result = 0.9*diffContrib +2.0*specContrib  + ambContrib +0.1*herolightfinal ;
    
    result.a=diffusemap.a * Alphavalue;

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

