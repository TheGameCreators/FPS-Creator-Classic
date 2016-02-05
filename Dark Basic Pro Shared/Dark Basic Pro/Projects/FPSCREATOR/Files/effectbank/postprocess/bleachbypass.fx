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
float4 PSbleachbypass( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	float4 base = tex2D(frameSamp, Tex);
    float3 lumCoeff = float3(0.25,0.65,0.1);
    float lum = dot(lumCoeff,base.rgb);
    float3 blend = lum.rrr;
    float L = min(1,max(0,10*(lum- 0.45)));
    float3 result1 = 2.0f * base.rgb * blend;
    float3 result2 = 1.0f - 2.0f*(1.0f-blend)*(1.0f-base.rgb);
    float3 newColor = lerp(result1,result2,L);
    
    float mix = 0.75;
    
    float A2 = mix* base.a;
    float3 mixRGB = A2 * newColor.rgb;
    mixRGB += ((1.0f-A2) * base.rgb);
    return float4(mixRGB,base.a);
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
		PixelShader = compile ps_2_0 PSbleachbypass();
	}
}



