/***************************************************************************************************************
This multi-technique shader provides an automatic adaptive bloom effect based on screen brightness to scale the bloom effect
Shader code by Mark Blosser
email: mjblosser@gmail.com
website: www.mjblosser.com
released under creative commons license by attribution: http://creativecommons.org/licenses/by-sa/3.0/

********************************************************************************************************************/

string Description = "Multi Post-Process Shader by bond1";

/**************UNTWEAKABLES******************************************************************************/

float2 ViewSize : ViewSize;
float time : Time;




//---------------RENDER TARGET TEXTURES--------------------------------------//

//starting scene image
texture frame : RENDERCOLORTARGET
< 
	string ResourceName = "";
	float2 ViewportRatio = {1.0,1.0 };
	
>;

sampler2D frameSamp = sampler_state {
    Texture = < frame >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

//starting scene image
texture frame2 : RENDERCOLORTARGET
< 
	string ResourceName = "";
	float2 ViewportRatio = {1.0,1.0 };
	
>;

sampler2D frameSamp2 = sampler_state {
    Texture = < frame2 >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

//starting scene image
texture frame3 : RENDERCOLORTARGET
< 
	string ResourceName = "";
	float2 ViewportRatio = {1.0,1.0 };
	
>;

sampler2D frameSamp3 = sampler_state {
    Texture = < frame3 >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};




//-------------STRUCTS-------------------------------------------------------------

struct input 
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};
 
struct output {

	float4 pos: POSITION;
	float2 uv: TEXCOORD0;

};

/*********************COMMON VERTEX SHADER************************************************/

output VS( input IN ) 
{
	output OUT;

	//quad needs to be shifted by half a pixel.
    //Go here for more info: http://www.sjbrown.co.uk/?article=directx_texels
    
	float4 oPos = float4( IN.pos.xy + float2( -1.0f/ViewSize.x, 1.0f/ViewSize.y ),0.0,1.0 );
	OUT.pos = oPos;

	float2 uv = (IN.pos.xy + 1.0) / 2.0;
	uv.y = 1 - uv.y; 
	OUT.uv = uv;
	
	return OUT;	
}

/*******************PIXEL SHADERS****************************************************/

//SSAA
float4 PSssaa( output IN, uniform sampler2D srcTex ) : COLOR
{
    float4 color = 0; 
    
    float2 ViewInv = float2( 1/ViewSize.x,1/ViewSize.y);
    float w = 1.75;
    
    float3 t = tex2D(srcTex, IN.uv + float2(0.0, -1.0) * w * ViewInv).xyz;
    float tLum = dot(t,float3(0.212,0.716,0.072));
    float3 l = tex2D(srcTex, IN.uv + float2(-1.0, 0.0) * w * ViewInv).xyz;
    float lLum = dot(l,float3(0.212,0.716,0.072));
    float3 r = tex2D(srcTex, IN.uv + float2(1.0, 0.0) * w * ViewInv).xyz;
    float rLum = dot(r,float3(0.212,0.716,0.072));
    float3 b = tex2D(srcTex, IN.uv + float2(0.0, 1.0) * w * ViewInv).xyz;
    float bLum = dot(b,float3(0.212,0.716,0.072));
    
    float2 n = float2(-(tLum - bLum), rLum - lLum);
    float nl = length(n);
    
    if	(nl < (1.0 / 16.0))
     color = tex2D( srcTex, IN.uv );  
     
    else
    {
      n*= ViewInv/nl;
      
      float4 o = tex2D(srcTex, IN.uv ); 
      float4 t0 = tex2D(srcTex, IN.uv + n * 0.5) * 0.9 ;  
      float4 t1 = tex2D(srcTex, IN.uv - n * 0.5) * 0.9 ;  
      float4 t2 = tex2D(srcTex, IN.uv + n ) * 0.75 ;   
      float4 t3 = tex2D(srcTex, IN.uv - n ) * 0.75 ; 
      
      color =    float4(o + t0 + t1 + t2 + t3) / 4.3;
    
    
    } 
    
    
    
    return color;    
}






//-----------No effect pixel shader---------------------------------------------
float4 PSnone( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	// sample the source
	float4 cTextureScreen = tex2D( frameSamp, Tex.xy);
	
	return float4(cTextureScreen.xyz, cTextureScreen.a);
} 






/*********************TECHNIQUES**********************************************************************/


technique onepass
<
	//the initial texture we want DarkBASIC Professional to place the scene
	string RenderColorTarget = "frame";
>
{
	pass Present
	<
		
		string RenderColorTarget = "";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSssaa(frameSamp);
	}
	
	
}



technique twopass
<
	//the initial texture we want DarkBASIC Professional to place the scene
	string RenderColorTarget = "frame";
>
{
	pass Present
	<
		
		string RenderColorTarget = "frame2";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSssaa(frameSamp);
	}
	
	pass Present
	<
		//use a blank render target to draw to the screen
		string RenderColorTarget = "";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSssaa(frameSamp2);
	}
}



