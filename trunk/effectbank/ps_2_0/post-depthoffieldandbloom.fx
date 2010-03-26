string Description = "This shader blurs depth of field objects based on their distance from the camera";
string Thumbnail = "DepthOfField.png";

float2 ViewSize : ViewSize;

//box filter, declare in pixel offsets convert to texel offsets in PS
float2 PixelOffsets[9] =
{
    { -1,  -1 },
    { -1,  0  },
    { -1,  1  },
    { 0,   1  },
    { 1,   1  },
    { 1,   0  },
    { 1,   -1 },
    { 0,   -1 },
    { 0,   0  },
};

float blurSize
<
	string UIWidget = "slider";
	float UIMax = 5.0;
	float UIMin = 0.1;
	float UIStep = 0.001;
> = 1.500000;

float focalDistance
<
	string UIWidget = "slider";
	float UIMax = 1.0;
	float UIMin = 0.0;
	float UIStep = 0.001;
> = 0.25;

float BloomScale 
<
	string UIWidget = "slider";
	float UIMax = 2.0;
	float UIMin = 0.0;
	float UIStep = 0.01;
> = 2.0f;

float BloomPower
<
	string UIWidget = "slider";
	float UIMax = 4.0;
	float UIMin = 0.4;
	float UIStep = 0.01;
> = 1.75f;

//scene image
texture frame
< 
	string ResourceName = "";
>;

sampler2D frameSamp = sampler_state {
    Texture = < frame >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

//downsample image
texture Downsample1Img : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	int width = 1024;
	int height = 768;
>;

sampler2D Downsample1Samp = sampler_state {
    Texture = < Downsample1Img >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

//downsample image
texture Downsample2Img : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	int width = 1024;
	int height = 768;
>;

sampler2D Downsample2Samp = sampler_state {
    Texture = < Downsample2Img >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

texture Downsample3Img : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	int width = 640;
	int height = 480;
>;

sampler2D Downsample3Samp = sampler_state {
    Texture = < Downsample3Img >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

texture Downsample4Img : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	int width = 192;
	int height = 144;
>;

sampler2D Downsample4Samp = sampler_state {
    Texture = < Downsample4Img >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

//blur image1
texture Blur1Img : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	int width = 192;
	int height = 144;
>;

sampler2D Blur1Samp = sampler_state {
    Texture = < Blur1Img >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

//blur image2
texture Blur2Img : RENDERCOLORTARGET 
< 
	string ResourceName = ""; 
	int width = 192;
	int height = 144;
>;

sampler2D Blur2Samp = sampler_state {
    Texture = < Blur2Img >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

struct input 
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};
 
struct output {

	float4 pos: POSITION;
	float2 uv: TEXCOORD0;

};

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

//downsample used to blur the image
float4 PSDownsample( output IN, uniform sampler2D srcTex, uniform float reduceRatio ) : COLOR
{
    float4 color = 0;
    float2 scale = (reduceRatio*blurSize)/ViewSize;
    float4 center = tex2D( srcTex, IN.uv );

    // get the depth from the alpha channel (it has been divided by two and inverted)
    float a_depth = 1.0 - min(center.a * 2.0, 1);

    // work out how close it is to the focal point
    float alpha1 = (abs ( a_depth - focalDistance ) * 2);

    // also increase blur exponentially the closer to the camera you get
    float close = pow(max((1-a_depth)-0.75,0),3)*50;
    alpha1 += close;

    // blur the original color with the surrounding texels
    for (int i = 0; i < 9; i++)
    {
        //convert pixel offsets into texel offsets via the inverse view values.
        float4 newColor = tex2D( srcTex, IN.uv + PixelOffsets[i].xy*scale*alpha1 );
        color += newColor;
    }

    // output blurred final pixel but forward original alpha value
    return float4(color.xyz/9,center.a);

    // useful alternative output for debugging focal point distance
    //return float4(alpha1,alpha1,alpha1,center.a);  
    //return float4(alpha1,color.y/9,color.z/9,center.a);  
}

float4 PSCombine( input IN ) : COLOR 
{
    float3 color = tex2D( frameSamp, IN.uv );
    
    color += pow(tex2D( Blur2Samp, IN.uv ),BloomPower) * BloomScale;

    return float4( color, 1.0f );      
}

technique Blur
<
	string RenderColorTarget = "frame";
>
{
	pass Downsample
	<
		string RenderColorTarget = "Downsample1Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_a PSDownsample( frameSamp, 1.0 );
	}
	
	pass Downsample2
	<
		string RenderColorTarget = "Downsample2Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_a PSDownsample( Downsample1Samp, 1.0 );
	}

	pass Downsample3
	<
		string RenderColorTarget = "Downsample3Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSDownsample( Downsample2Samp, 0.25 );
	}

	pass Downsample4
	<
		string RenderColorTarget = "Downsample4Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSDownsample( Downsample3Samp, 0.25 );
	}
	
	pass BlurH
	<
		string RenderColorTarget = "Blur1Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSBlurH( Downsample4Samp );
	}
	
	pass BlurV
	<
		string RenderColorTarget = "Blur2Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSBlurV( Blur1Samp );
	}
	
	pass BlurH
	<
		string RenderColorTarget = "Blur1Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSBlurH( Blur2Samp );
	}
	
	pass BlurV
	<
		string RenderColorTarget = "Blur2Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSBlurV( Blur1Samp );
	}
	
	pass Combine
	<
		string RenderColorTarget = "";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSCombine();
	}
}
