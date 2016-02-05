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

texture multi1 : DIFFUSE 
< 
   string ResourceName = ""; 
>;

sampler multi1Samp = sampler_state
{
	Texture = <multi1>;
	MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
	AddressU = Wrap; AddressV = Wrap;
};

texture multi2 : DIFFUSE 
< 
   string ResourceName = ""; 
>;

sampler multi2Samp = sampler_state
{
	Texture = <multi2>;
	MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
	AddressU = Wrap; AddressV = Wrap;
};

texture frame2 : RENDERCOLORTARGET
< 
	string ResourceName = "";
	float2 ViewportRatio = {1.0,1.0 };
	
>;

sampler2D frameSamp2 = sampler_state 
{
    Texture = < frame2 >;
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

//-----------rain without refraction---------------------------------------------
float4 PSrain( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	

    float4 frame = tex2D(frameSamp,Tex);
    
    float4 rain = tex2D(multi1Samp,2*Tex + float2(.05,-7)*time);
    
    return frame + 0.075*float4(rain.xxx,0);
    
   
} 



//-----------rain plus refraction---------------------------------------------
float4 PSrainrefract( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	//Code chunk to keep time value from getting too high/////////////////////
    float looptime = 50; //looptime in seconds
	float loopcounter  = floor(time/looptime); //increments by one every 50 seconds (or whatever "looptime" is)
	float offset ;  //initialize offset value used below
	offset = looptime*loopcounter; //offset time value -increments every looptime
	float speed =(time) - (offset) ;
    ///////////////////////////////////////////////////////////////////////////
    
    
    float2 displace = float2(Tex.x*3,Tex.y*2);
	
	float timer =  speed*7;
	//float timer = 1; //no movement
	
	displace.x += (timer*.01);  //left to right refraction can also add a sin here to mix it up
	displace.y -= timer;  //downward refraction	
	//displace = displace-timer;
	
	float refractscale = .005;  //0.01
	
	
	float4 Normalmap = (tex2D(multi2Samp,displace.xy)*2-1) ;
	float2 displaceUV = Normalmap * refractscale; 
	
	// sample the source
	float4 cTextureScreen = tex2D( frameSamp, Tex.xy + displaceUV);
	
	float4 rain = tex2D(multi1Samp,displace.xy);
	
	return float4(cTextureScreen.xyz, cTextureScreen.a) + 0.07*rain  ;
} 

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

 

/*********************TECHNIQUES**********************************************************************/


technique rainrefract_ssaa
<
	//the initial texture we want DarkBASIC Professional to place the scene
	string RenderColorTarget = "frame";
>
{
	pass rain
	<
		//use a blank render target to draw to the screen
		string RenderColorTarget = "frame2";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSrainrefract();
	}
	
	pass ssaa
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


technique rainrefract
<
	//the initial texture we want DarkBASIC Professional to place the scene
	string RenderColorTarget = "frame";
>
{
	pass rain
	<
		//use a blank render target to draw to the screen
		string RenderColorTarget = "";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSrainrefract();
	}
}

technique simplerain
<
	//the initial texture we want DarkBASIC Professional to place the scene
	string RenderColorTarget = "frame";
>
{
	pass rain
	<
		//use a blank render target to draw to the screen
		string RenderColorTarget = "";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSrain();
	}
}




