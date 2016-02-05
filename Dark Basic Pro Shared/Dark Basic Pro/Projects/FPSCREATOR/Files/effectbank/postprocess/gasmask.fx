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

sampler2D frameSamp = sampler_state 
{
    Texture = < frame >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

texture Normalmap : DIFFUSE 
< 
   string ResourceName = ""; 
>;

sampler NormalmapSamp = sampler_state
{
	Texture = <Normalmap>;
	MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
	AddressU = Wrap; AddressV = Wrap;
};

texture Dirtmap : DIFFUSE 
< 
   string ResourceName = ""; 
>;

sampler DirtmapSamp = sampler_state
{
	Texture = <Dirtmap>;
	MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
	AddressU = Wrap; AddressV = Wrap;
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



//-----------No effect pixel shader---------------------------------------------
float4 PSnone( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	
	float2 displace = Tex;
	
	//float timer =  time*.1;
	float timer = 1; //no movement
	
	//displace.x -= timer;  //left to right refraction
	//displace.y -= timer;  //downward refraction	
	displace = displace-timer;
	
	float refractscale = .05;
	
	
	float4 Normalmap = (tex2D(NormalmapSamp,displace.xy)*2-1) ;
	float2 displaceUV = Normalmap * refractscale; 
	
	// sample the source
	float4 cTextureScreen = tex2D( frameSamp, Tex.xy + displaceUV);
	float4 dirtmap = tex2D( DirtmapSamp, Tex.xy + displaceUV);
	
	//vignette
  float2 vpos = (Tex.xy - float2(.5,.5)) * 2 ;
  //vpos.x = vpos.x * -1.5 - 0; //can also scale and shift horizontally
  float dist = (dot(vpos, vpos)) ;
  dist =  1-0.3 * dist;
	
	return float4(0.8*cTextureScreen.xyz*dirtmap.xyz+(0.25*dirtmap.xyz*cTextureScreen.xyz), cTextureScreen.a) ;
} 


 







/*********************TECHNIQUES**********************************************************************/


technique none
<
	//the initial texture we want DarkBASIC Professional to place the scene
	string RenderColorTarget = "frame";
>
{
	pass Present
	<
		//use a blank render target to draw to the screen
		string RenderColorTarget = "";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSnone();
	}
}



