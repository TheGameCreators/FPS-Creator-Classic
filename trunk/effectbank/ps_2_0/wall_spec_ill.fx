

//Description:  Shader for lightmapped segment walls.  Uses constant forward "hero" specular highlights
//Textures: 
//"texture_D.dds"      diffuse texture
//"texture_I.dds"       specular texture, alpha for illumination


/************* UNTWEAKABLES **************/

float4x4 World      : WORLD;

float4x4 WorldViewProj : WorldViewProjection;
float4x4 WorldIT : WorldInverseTranspose;
float4x4 ViewInv : ViewInverse;
float4 eyePos : CameraPosition;




/******TWEAKABLES***************************/

float SpecExpon : Power
<
    string UIWidget = "slider";
    float UIMin = 1.0;
    float UIMax = 128.0;
    float UIStep = 1.0;
    string UIName =  "specular power";
> = 64.0;


/******VALUES PULLED FROM FPSC - NON TWEAKABLE**********/

float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
> = {0.1f, 0.1f, 0.1f, 1.0f};

float4 SurfColor : Diffuse
<
    string UIName =  "Surface Color";
    string UIType = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float4 LightSource
<
    string UIType = "Fixed Light Source";
> = {5000.0f,100.0f, -0.0f, 1.0f};




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

//could be anything here - ill,spec,normal,cube
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
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//Diffuse Texture _D
sampler2D DiffuseSampler = sampler_state
{
    Texture   = <DiffuseMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//Effect Texture _I (specular texture)
sampler2D EffectSampler = sampler_state
{
    Texture   = <EffectMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


;





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
                                           

    return OUT;
}


/****************Framgent Shader*****************/

float4 mainPS(vertexOutput IN) : COLOR
{
    float4 LM = tex2D(LightmapSampler,IN.TexCoordLM);  //sample lightmap texture
    float4 diffuse = tex2D(DiffuseSampler,IN.TexCoord.xy);    //sample diffuse texture    
    float4 effectmap = tex2D(EffectSampler,IN.TexCoord.xy);    //sample specular map texture 
    
    float3 Ln = (IN.LightVec);
    float3 Nn = normalize(IN.WorldNormal);
   
     
    
    //create normalized view vector for constant forward "hero" spec
    float3 V  = normalize(eyePos - IN.WPos);
	
        
    //half vector
    float3 Hn = normalize(V+Ln);
    float dis = distance(IN.WPos,eyePos);

    float atten = (1/(dis*(dis*.01)))* 2000 ;  //last value is multiplier, inverse square faloff
	atten = clamp(atten,0,1.5);  //second value controls how bright to let the highlights become
	
	

    
    
   //specular highlights 
    float herospec = pow(max(dot(Nn,Hn),0),48);
    
    
    //multiply spec texture, lightmap, and diffuse texture
    float4 specular = (herospec)*(effectmap+0.1)*5*(LM+0.1)*diffuse * atten;
    
        
    
    float4 LMfinal = (LM + AmbiColor)*diffuse  ;
    
        
    float4 result =   LMfinal  + specular + (effectmap.w * diffuse)  ;
    
    return result;
}


/****** technique ********************************/

technique dx9textured
{
    pass P0
    {
        // lighting
        Lighting       = FALSE;
        FogEnable      = FALSE;

        // samplers
        //Sampler[0] = (LightmapSampler);
        //Sampler[1] = (DiffuseSampler);
        //Sampler[2] = (IllSpecSampler);
		//Sampler[3] = (NormalSampler);

        // shaders
        VertexShader = compile vs_2_0 mainVS();
        PixelShader  = compile ps_2_0 mainPS();
    }
}
