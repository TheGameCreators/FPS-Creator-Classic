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

/***************BLOOM TWEAKS***************************************************************************/

float PreBloomBoost 
<
	string UIWidget = "slider";
	float UIMax = 4.0;
	float UIMin = 0.0;
	float UIStep = 0.1;
> = 2.0f;

float BloomThreshold 
<
	string UIWidget = "slider";
	float UIMax = 1.0;
	float UIMin = 0.0;
	float UIStep = 0.05;
> = 0.4;

float PostBloomBoost 
<
	string UIWidget = "slider";
	float UIMax = 4.0;
	float UIMin = 0.0;
	float UIStep = 0.1;
> = 2.0f;

float BloomWidth 
<
	string UIWidget = "slider";
	float UIMax = 6.0;
	float UIMin = 1.0;
	float UIStep = 0.1;
> = 2.5f;


//9 sample gauss filter, declare in pixel offsets convert to texel offsets in PS
float4 GaussFilter[9] =
{
    { -1,  -1, 0,  0.0625 },
    { -1,   1, 0,  0.0625 },
    {  1,  -1, 0,  0.0625 },
    {  1,   1, 0,  0.0625 },
    { -1,   0, 0,  0.125  },
    {  1,   0, 0,  0.125  },
    {  0,  -1, 0,  0.125 },
    {  0,   1, 0,  0.125 },
    {  0,   0, 0,  0.25 },
};


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

//2x2 average luminosity texture using point sampling
texture AvgLum2x2Img : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	//float2 ViewportRatio = { 0.5, 0.5 };
	int width = 2;
	int height = 2;
>;

sampler2D AvgLum2x2ImgSamp = sampler_state {
    Texture = < AvgLum2x2Img >;
    MinFilter = Point; MagFilter = Point; MipFilter = Point;
    AddressU = Clamp; AddressV = Clamp;
};

//Average scene luminosity stored in 1x1 texture 
texture AvgLumFinal : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	//float2 ViewportRatio = { 0.5, 0.5 };
	int width = 1;
	int height = 1;
>;

sampler2D AvgLumFinalSamp = sampler_state {
    Texture = < AvgLumFinal >;
    MinFilter = Point; MagFilter = Point; MipFilter = Point;
    AddressU = Clamp; AddressV = Clamp;
};

//reduce image to 1/8 size for brightpass
texture BrightpassImg : RENDERCOLORTARGET
< 
	string ResourceName = "";
	//float2 ViewportRatio = {0.125,0.125 };
	int width = 512;
	int height = 384;
	
>;

sampler2D BrightpassImgSamp = sampler_state {
    Texture = < BrightpassImg >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

//blur texture 1
texture Blur1Img : RENDERCOLORTARGET
< 
	string ResourceName = "";
	//float2 ViewportRatio = {0.125,0.125 };
	int width = 512;
	int height = 384;
	
>;

sampler2D Blur1ImgSamp = sampler_state {
    Texture = < Blur1Img >;
    MinFilter = Anisotropic; MagFilter = Anisotropic; MipFilter = Anisotropic;
    AddressU = Clamp; AddressV = Clamp;
};

//blur texture 2
texture Blur2Img : RENDERCOLORTARGET
< 
	string ResourceName = "";
	//float2 ViewportRatio = {0.125,0.125 };
	int width = 512;
	int height = 384;
	
>;

sampler2D Blur2ImgSamp = sampler_state {
    Texture = < Blur2Img >;
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

//takes original frame image and outputs to 2x2
float4 PSReduce( output IN, uniform sampler2D srcTex ) : COLOR
{
    float4 color = 0;    
    color= tex2D( srcTex, IN.uv );    
    return color;    
}

//-----------------computes average luminosity for scene-----------------------------
float4 PSGlareAmount( output IN, uniform sampler2D srcTex ) : COLOR
{
    float4 GlareAmount = 0;
    
    //sample texture 4 times with offset texture coordinates
    float4 color1= tex2D( srcTex, IN.uv + float2(-0.5, -0.5) );
    float4 color2= tex2D( srcTex, IN.uv + float2(-0.5, 0.5) );
    float4 color3= tex2D( srcTex, IN.uv + float2(0.5, -0.5) );
    float4 color4= tex2D( srcTex, IN.uv + float2(0.5, 0.5) );
    
    //average these samples
    float3 AvgColor = saturate(color1.xyz * 0.25 + color2.xyz * 0.25 + color3.xyz * 0.25 + color4.xyz * 0.25);
    
    //exxagerate values to make effect more evident
    AvgColor = 8*AvgColor*AvgColor;
    
    //convert to luminance
    AvgColor = dot(float3(0.3,0.59,0.11), AvgColor);
    
    GlareAmount.xyz = AvgColor;
    
    //interpolation value to blend with previous frames
    GlareAmount.w = 0.03;
       
    return GlareAmount;    
}


//-------------brightpass pixel shader, removes low luminance pixels--------------------------
float4 PSBrightpass( output IN, uniform sampler2D srcTex, uniform sampler2D srcTex2  ) : COLOR
{
    float4 screen = tex2D(srcTex, IN.uv);  //original screen texture;
    float4 glaretex = tex2D(srcTex2, IN.uv);  //glareamount from 1x1 in previous pass;
    
    //remove low luminance pixels, keeping only brightest
    float3 Brightest = saturate(screen.xyz - BloomThreshold) * PreBloomBoost;
    //Brightest.xyz = pow(Brightest.xyz,2) * 2;
    
    //apply glare adaption
    //float3 adapt = Brightest.xyz * (1-glaretex.xyz);
    
    float4 color = float4(Brightest.xyz, 1);
    
    return color;    
}

float4 PSBlur( output IN, uniform sampler2D srcTex ) : COLOR
{
    float4 color = float4(0,0,0,0);
    
    //inverse view for correct pixel to texel mapping
    float2 ViewInv = float2( 1/ViewSize.x,1/ViewSize.y);   
    
    //sample and output average colors using gauss filter samples
    for(int i=0;i<9;i++)
    
    {
    //float4 col = GaussFilter[i].w * tex2D(srcTex,IN.uv + float2(GaussFilter[i].x * ViewInv.x*width, GaussFilter[i].y *ViewInv.y*width));  
    float4 col = GaussFilter[i].w * tex2D(srcTex,IN.uv + float2(GaussFilter[i].x * ViewInv.x*BloomWidth, GaussFilter[i].y *ViewInv.y*BloomWidth));  
    color+=col;
    }
    return color;
}

//combine adaptive bloom texture with original scene image
float4 PSPresent( output IN, uniform sampler2D srcTex, uniform sampler2D srcTex2, uniform sampler2D srcTex3 ) : COLOR
{
    float4 color = float4(0,0,0,0);
    
    //sample screen texture and bloom texture
    float4 ScreenMap=tex2D(srcTex, IN.uv);
    float4 BloomMap=tex2D(srcTex2, IN.uv);
    float4 AmtMap=tex2D(srcTex3, IN.uv);
    
    //technique 1: just add results
    
    //float Luminance = dot(ScreenMap.xyz, float3(0.3, 0.59, 0.11));
    //float3 mod = lerp(PostBloomBoost,0,AmtMap.x);
    //float3 final = ScreenMap + 0.25*ScreenMap*mod + mod*BloomMap;
    //final  = lerp(final, ScreenMap+final*.1, Luminance);
    //float3 final = BloomMap;
    
    //technique 2: add results based scene pixel brightness
    float Luminance = dot(ScreenMap.xyz, float3(0.3, 0.59, 0.11));
    float4 MaxAmount= ScreenMap + BloomMap;
    float3 final=lerp(MaxAmount*1.0, ScreenMap, Luminance);
    
    //technique 3: inverse multiplication
    //float3 final=(ScreenMap.xyz + BloomMap.xyz * (1-ScreenMap.xyz));
    
    color = float4(final,1);
    
    return color;
        
}

//-----------No effect pixel shader---------------------------------------------
float4 PSnone( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	// sample the source
	float4 cTextureScreen = tex2D( frameSamp, Tex.xy);
	
	return float4(cTextureScreen.xyz, cTextureScreen.a);
} 

//----------------Nightvision pixel shader----------------------------------------
float4 PSnightvision( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	float4 cTextureScreen = tex2D(Blur1ImgSamp, Tex.xy).rgba;
	//float4 cTextureScreen = tex2D(frameSamp, Tex.xy).rgba;
	/*
	//blur whole screen
   float4 blurtotal = float4(0, 0, 0, 0);
   float blursize = 1.5;
   float2 ViewInv = float2( 1/ViewSize.x,1/ViewSize.y); 
   blurtotal += tex2D(frameSamp, Tex.xy + float2(-blursize*ViewInv.x, -blursize*ViewInv.y));
   blurtotal += tex2D(frameSamp, Tex.xy + float2(-blursize*ViewInv.x,  blursize*ViewInv.y));
   blurtotal += tex2D(frameSamp, Tex.xy + float2( blursize*ViewInv.x, -blursize*ViewInv.y));
   blurtotal += tex2D(frameSamp, Tex.xy + float2( blursize*ViewInv.x,  blursize*ViewInv.y));
   blurtotal += tex2D(frameSamp, Tex.xy + float2(-blursize*ViewInv.x,  0*ViewInv.y));
   blurtotal += tex2D(frameSamp, Tex.xy + float2( blursize*ViewInv.x,  0*ViewInv.y));
   blurtotal += tex2D(frameSamp, Tex.xy + float2( 0*ViewInv.x, -blursize*ViewInv.y));
   blurtotal += tex2D(frameSamp, Tex.xy + float2( 0*ViewInv.x,  blursize*ViewInv.y));
   blurtotal *= 0.125;
   cTextureScreen = blurtotal;
	*/
	
	//-------------prevent time value from becoming too large--------------
	//float framespersec = 25;
	float looptime = 100; //looptime in seconds			
	float loopcounter  = floor(time/looptime); //increments by one every 50 seconds (or whatever "looptime" is)
	float offset ;  //initialize offset value used below	
	offset = looptime*loopcounter; //offset time value -increments every looptime
	//float speed =(time*framespersec) - (offset*framespersec) ;	 
	float speed =(time) - (offset) ;
	//------------------------------------------------------------------------
	
	
	float x = Tex.x * Tex.y *(speed*100);
	x = fmod(x, 13) * fmod(x, 123);	
	float dx = fmod(x, 0.01);

	float3 cResult = cTextureScreen.rgb + cTextureScreen.rgb * saturate(0.1f + dx.xxx * 100);  //noise
	//float3 cResult = cTextureScreen.rgb ;

	float2 sc; sincos(Tex.y * 768-(speed*50), sc.x, sc.y);//scrolling scanlines (remember faster in FPSC!)
	float2 bc; sincos(Tex.y -(speed*50), bc.x, bc.y); //blinking screen (remember faster in FPSC!)

	cResult += cTextureScreen.rgb * float3(sc.x, sc.y, sc.x) * 0.7 + 0.05*float3(bc.x, bc.y, bc.x);
	
	// interpolate between source and result by intensity
	cResult = lerp(cTextureScreen, cResult, saturate(0.3));
		
	cTextureScreen = float4(cResult,1);
	
	float green = cTextureScreen.g;
	
    green = cTextureScreen.r + cTextureScreen.b;
 
  //vignette
  float2 vpos = (Tex.xy - float2(.5,.5)) * 2.2 ;
  //vpos.x = vpos.x * -1.5 - 0; //can also scale and shift horizontally
  float dist = (dot(vpos, vpos)) + 0.2;
  dist =  1-0.4 * dist;
  
  return float4(0,green,0,1) * dist;
  //return float4(cTextureScreen ) * dist;
} 

//------------------Toon pixel shader--------------------------------
float4 PStoon( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	float2 ViewInv = float2( 1/ViewSize.x,1/ViewSize.y); 
	float4 finalcolor;
	float4 to_gray = float4(0.3,0.59,0.11,0);
  float x1 = dot(to_gray, tex2D(frameSamp, Tex.xy+float2(1*ViewInv.x,1*ViewInv.y)));
  float x0 = dot(to_gray, tex2D(frameSamp, Tex.xy+float2(-1*ViewInv.x,-1*ViewInv.y)));
  float x3 = dot(to_gray, tex2D(frameSamp, Tex.xy+float2(1*ViewInv.x,-1*ViewInv.y)));
  float x2 = dot(to_gray, tex2D(frameSamp, Tex.xy+float2(-1*ViewInv.x,1*ViewInv.y)));
  float edge = (x1 - x0) * (x1 - x0);
  float edge2 = (x3 - x2) * (x3 - x2);
  edge += edge2;
  // sample the source
	float4 cTextureScreen = tex2D( frameSamp, Tex.xy);
	

  finalcolor= float4(max(cTextureScreen - float4(edge, edge, edge, edge) * 12, float4(0,0,0,0)));
  return float4(finalcolor.xyz,1);
  
  
  /*madworld
  float4 ocol0;
  float4 emboss = (tex2D(frameSamp, Tex.xy+1*ViewInv).rgba - tex2D(frameSamp, Tex.xy-1*ViewInv).rgba)*2.0f;
  emboss -= (tex2D(frameSamp, Tex.xy+float2(1*ViewInv.x,-1*ViewInv.y)).rgba - tex2D(frameSamp, Tex.xy+float2(-1*ViewInv.x,1*ViewInv.y)).rgba);
  float4 color = tex2D(frameSamp, Tex.xy).rgba;
  if (color.r > 0.8 && color.b + color.b < 0.2)
    ocol0 = float4(1,0,0,0);
  else {
    color += emboss;
    if (dot(color.rgb, float3(0.3, 0.5, 0.2)) > 0.5)
      ocol0 = float4(1,1,1,1);
    else
      ocol0 = float4(0,0,0,0); 
      }
      return float4(ocol0);
      */
} 

//-------------Dreamy Pixel Shader------------------------------------
float4 PSdreamy( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	float4 finalcolor;
	
	// sample the source
	float4 cTextureScreen = tex2D( frameSamp, Tex.xy);
	
	float2 ViewInv = float2( 1/ViewSize.x,1/ViewSize.y); 
	

  float4 bloom_sum = float4(0.0, 0.0, 0.0, 0.0);
  
  float radius1 = 1.3;
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(-1.5*ViewInv.x, -1.5*ViewInv.y) * radius1);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(-2.5*ViewInv.x, 0*ViewInv.y)  * radius1);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(-1.5*ViewInv.x, 1.5*ViewInv.y) * radius1);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(0*ViewInv.x, 2.5*ViewInv.y) * radius1);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(1.5*ViewInv.x, 1.5*ViewInv.y) * radius1);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(2.5*ViewInv.x, 0*ViewInv.y) * radius1);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(1.5*ViewInv.x, -1.5*ViewInv.y) * radius1);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(0*ViewInv.x, -2.5*ViewInv.y) * radius1);

  float radius2 = 2.6;
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(-1.5*ViewInv.x, -1.5*ViewInv.y) * radius2);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(-2.5*ViewInv.x, 0*ViewInv.y)  * radius2);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(-1.5*ViewInv.x, 1.5*ViewInv.y)  * radius2);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(0*ViewInv.x, 2.5*ViewInv.y)  * radius2);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(1.5*ViewInv.x, 1.5*ViewInv.y)  * radius2);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(2.5*ViewInv.x, 0*ViewInv.y)  * radius2);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(1.5*ViewInv.x, -1.5*ViewInv.y)  * radius2);
  bloom_sum += tex2D(frameSamp, Tex.xy + float2(0*ViewInv.x, -2.5*ViewInv.y)  * radius2);

  bloom_sum *= 0.1;
  bloom_sum -= 0.3;
  bloom_sum = max(bloom_sum,0);

  //vignette
  float2 vpos = (Tex.xy - float2(.5,.5)) * 2 ;
  //vpos.x = vpos.x * -1.5 - 0; //can also scale and shift horizontally
  float dist = (dot(vpos, vpos)) ;
  dist =  1-0.4 * dist;

  finalcolor = float4(cTextureScreen  * 0.7 + bloom_sum) * dist;
  return float4(finalcolor);
} 

//-----------Monochrome---------------------------------------------
float4 PSmonochrome( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	// sample the source
	float4 cTextureScreen = tex2D( frameSamp, Tex.xy);
	
	cTextureScreen = dot(float3(0.3,0.59,0.11), cTextureScreen);
	
	return float4(cTextureScreen.xyz, cTextureScreen.a);
} 



/*********************TECHNIQUES**********************************************************************/

technique nightvistion
<
	//the initial texture we want DarkBASIC Professional to place the scene
	string RenderColorTarget = "frame";
>
{
	pass Reduce2x2
	<
		string RenderColorTarget = "Blur1Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSReduce(frameSamp);		
	}
	
	pass Present
	<
		//use a blank render target to draw to the screen
		string RenderColorTarget = "";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PSnightvision();
	}
}
