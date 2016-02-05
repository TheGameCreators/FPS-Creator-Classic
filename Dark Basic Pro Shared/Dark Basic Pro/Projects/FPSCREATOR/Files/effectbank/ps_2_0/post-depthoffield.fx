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
> = 1.000000;

float focalDistance
<
	string UIWidget = "slider";
	float UIMax = 1.0;
	float UIMin = 0.0;
	float UIStep = 0.001;
> = 0.25;

// main scene camera
texture frame
< 
	string ResourceName = "";
>;
sampler2D frameSamp = sampler_state {
    Texture = < frame >;
    MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
    AddressU = Clamp; AddressV = Clamp;
};

// depth camera image (created using FOG)
texture framedepth
< 
	string ResourceName = "";
>;
sampler2D framedepthSamp = sampler_state {
    Texture = < framedepth >;
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
float4 PSDownsample( output IN, uniform sampler2D srcTex, uniform sampler2D srcDepthTex, uniform float reduceRatio ) : COLOR
{
    // displacement effect
    float4 depthinfo = tex2D( srcDepthTex, IN.uv );
    float2 displacement = float2(depthinfo.y-0.5,depthinfo.z-0.5);
    displacement *= (1-depthinfo.x);
    displacement /= 64.0;

    float4 color = 0;
    float2 scale = (reduceRatio*blurSize)/ViewSize;

    // get the depth from the depth camera image
    float a_depth = depthinfo.x;

    // work out how close it is to the focal point
    float alpha1 = (abs ( a_depth - focalDistance ) * 2);

    // (optional) increase blur exponentially the closer to the camera you get
    // float close = pow(max((1-a_depth)-0.75,0),3)*50;
    // alpha1 += close;

    // blur the original color with the surrounding texels
    for (int i = 0; i < 9; i++)
    {
        //convert pixel offsets into texel offsets via the inverse view values.
        float4 newColor = tex2D( srcTex, IN.uv + displacement + PixelOffsets[i].xy*scale*alpha1 );
        color += newColor;
    }

    // output blurred final pixel but forward original alpha value
    return float4(color.xyz/9,depthinfo.x);

    // useful alternative output for debugging focal point distance
    //return float4(alpha1,color.y,color.z,depthinfo.x);
}

//copy the low res downsample into a high res screen quad using bilinear filtering
float4 PSPresent( output IN, uniform sampler2D srcTex ) : COLOR
{
    float4 texcol = tex2D(srcTex,IN.uv);
    return texcol;
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
		PixelShader = compile ps_2_a PSDownsample( frameSamp, framedepthSamp, 1.0 );
	}
	
	pass Downsample2
	<
		string RenderColorTarget = "Downsample2Img";
	>
	{
		ZEnable = False;
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_a PSDownsample( Downsample1Samp, framedepthSamp, 1.0 );
	}

	pass Present
	<
		string RenderColorTarget = "";
	>
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PSPresent( Downsample1Samp );
	}
}
