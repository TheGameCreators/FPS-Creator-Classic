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

texture scratches : DIFFUSE 
< 
   string ResourceName = ""; 
>;

sampler scratchesSamp = sampler_state
{
	Texture = <scratches>;
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
float4 PSfilmreel( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	
	/****Calculate a set of texture coordinates to perform atlas (sprite) walking*********/
	float2 DimensionsXY = float2(4,4); 
	float2 atlasUVtemp = Tex; 
	float framespersec = 25;  //speed of animation in frames per second
	float looptime = 10; //looptime in seconds
			
	float loopcounter  = floor(time/looptime); //increments by one every 50 seconds (or whatever "looptime" is)
	float offset ;  //initialize offset value used below
	
	offset = looptime*loopcounter; //offset time value -increments every looptime
	
		 
	float speed =(time*framespersec) - (offset*framespersec) ;
	
	
	float index = floor( speed);		//floor of speed
	float rowCount = floor( (index / DimensionsXY.x) );		//floor of (speed / Ydimension.g)
	float2 offsetVector = float2(index, rowCount);
	float2 atlas = (1.0 / DimensionsXY) ;
	float2 move = (offsetVector + atlasUVtemp);
	
	float2 atlasUV = (atlas.xy *move);
	/*******************************************************************************************/ 
	
	float4 scratch = tex2D(scratchesSamp, atlasUV);
	
	float4 base = tex2D(frameSamp, Tex);
    float3 lumCoeff = float3(0.3,0.59,0.11);
    float3 lum = dot(lumCoeff,base.rgb);
    base = float4(lum.rgb,1); //make completely grayscale
    float3 blend = lum.rgb;
    float L = min(1,max(0,10*(lum- 0.45)));
    //float3 result1 = 2.0f * base.rgb * blend;
    float3 result1 = 2.5f * base.rgb * blend;
    float3 result2 = 1.0f - 2.0f*(1.0f-blend)*(1.0f-base.rgb);
    float3 newColor = lerp(result1,result2,L);
    
    float mix = 0.75;
    
    float A2 = mix* base.a;
    float3 mixRGB = A2 * newColor.rgb;
    mixRGB += ((1.0f-A2) * base.rgb);
    
    //vignette
  float2 vpos = (Tex.xy - float2(.5,.5)) * 2 ;
  //vpos.x = vpos.x * -1.5 - 0; //can also scale and shift horizontally
  float dist = (dot(vpos, vpos)) ;
  dist =  1-0.4 * dist;
  
    
    return (float4(mixRGB,base.a)*dist + 1*scratch)   ;
    
    
    
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
		PixelShader = compile ps_2_0 PSfilmreel();
	}
}



