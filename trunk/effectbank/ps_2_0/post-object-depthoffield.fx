string Description = "This shader provides data for a depth of field fullscreen shader.";
string Thumbnail = "DepthOfField.png";

float4x4 WorldViewProj : WorldViewProjection;
float4x4 WorldView : WorldView;
float4x4 World : World;
float4x4 WorldIT : WorldInverseTranspose;

float4 LightPos[8] : LIGHTPOSITION;
float3 LightColor[8] : LIGHTCOLOR;
float3 AmbientColor : AMBIENTCOLOR;

float time : TIME;

float depthScale
<
	string UIWidget = "slider";
	float UIMax = 0.1;
	float UIMin = 0.001;
	float UIStep = 0.001;
> = 0.015;

texture BaseTex
<
	string ResourceName = "";
>;

texture BaseTex2
<
	string ResourceName = "";
>;

sampler diffuse_smp = sampler_state 
{
	Texture = <BaseTex>;
	MinFilter = Anisotropic;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler diffuse_smp2 = sampler_state 
{
	Texture = <BaseTex2>;
	MinFilter = Anisotropic;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct app_in
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
};

struct vs_out
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
	float3 normal : TEXCOORD2;
	float4 wpos : TEXCOORD3;
	float4 ppos : TEXCOORD4;
};

vs_out VS( app_in IN )
{
	vs_out OUT;
	
	float4 pos = mul( IN.pos, WorldViewProj );
	OUT.pos = pos;
	OUT.ppos = mul( IN.pos, WorldView );
	OUT.wpos = mul( IN.pos, World );
	OUT.uv = IN.uv;
	OUT.uv2 = IN.uv2;
	OUT.normal = mul(IN.normal.xyz,(float3x3)World );
	
	return OUT;
}

float4 PS( vs_out IN, uniform int numLights ) : COLOR
{
	float4 texColor = tex2D( diffuse_smp, IN.uv );
	return texColor;
}
float4 PSdepth( vs_out IN, uniform int numLights ) : COLOR
{
	float4 texColor = tex2D( diffuse_smp, IN.uv );
	float a_depth = 1.0 - ( (depthScale/10.0f) * (IN.ppos.z / IN.ppos.w) );
	return float4(texColor.rgb, a_depth / 2.0f );
}
float4 PSLM( vs_out IN, uniform int numLights ) : COLOR
{
	float4 LM = tex2D( diffuse_smp, IN.uv2 );
	float4 texColor = tex2D( diffuse_smp2, IN.uv ) * LM;
	return texColor;
}
float4 PSLMdepth( vs_out IN, uniform int numLights ) : COLOR
{
	float4 LM = tex2D( diffuse_smp, IN.uv2 );
	float4 texColor = tex2D( diffuse_smp2, IN.uv ) * LM;
	
        // we invert the depth value so transparency fades out over distance (inverted again at post)
	float a_depth = 1.0 - ( (depthScale/10.0f) * (IN.ppos.z / IN.ppos.w) );

        // finally divide alpha value by two (so when transparency used it looks see-through)
	return float4(texColor.rgb, a_depth / 2.0f );
}

technique alpha
{
	pass p0
	{
		VertexShader = compile vs_1_1 VS( );
		PixelShader = compile ps_2_0 PS( 0 );
	}
}
technique depth
{
	pass p0
	{
		AlphaBlendEnable = FALSE;
		AlphaTestEnable = FALSE;
		VertexShader = compile vs_1_1 VS( );
		PixelShader = compile ps_2_0 PSdepth( 0 );
	}
}
technique alphaLM
{
	pass p0
	{
		VertexShader = compile vs_1_1 VS( );
		PixelShader = compile ps_2_0 PSLM( 0 );
	}
}
technique depthLM
{
	pass p0
	{
		AlphaBlendEnable = FALSE;
		AlphaTestEnable = FALSE;
		VertexShader = compile vs_1_1 VS( );
		PixelShader = compile ps_2_0 PSLMdepth( 0 );
	}
}
