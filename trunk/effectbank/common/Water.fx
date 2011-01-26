//====================================================
// Reflective/Refractive Water Shader 
//====================================================
// By EVOLVED
// www.evolved-software.com
// Modified by TGC (JAN 2011)
//====================================================

//--------------
// un-tweaks
//--------------
   matrix WorldVP:WorldViewProjection; 
   matrix World:World;   
   matrix ViewInv:ViewInverse; 
   float time:Time;

//--------------
// tweaks
//--------------
   float2 WaterScale={10.0f,10.0f};
   float WaterBump=0.02f;
   float FresPow=2.5f;
   float2 Speed1={-0.04,0.04};
   float2 Speed2={0.05,-0.05};
   float3 RefractColor={1.0f,1.0f,1.0f};
   float3 ReflectColor={1.0f,1.0f,1.0f};

//--------------
// Textures
//--------------
   texture WaterRefractTX <string Name=""; >;	
   sampler2D WaterRefract=sampler_state
     {
	Texture = <WaterRefractTX>;
   	ADDRESSU = CLAMP;
   	ADDRESSV = CLAMP;
   	ADDRESSW = CLAMP;
	MagFilter = None;	
	MinFilter = None;
	MipFilter = None;
     };
   texture WaterReflectTX <string Name=""; >;	
   sampler2D WaterReflect=sampler_state
     {
	Texture = <WaterReflectTX>;
   	ADDRESSU = CLAMP;
   	ADDRESSV = CLAMP;
   	ADDRESSW = CLAMP;
	MagFilter = None;	
	MinFilter = None;
	MipFilter = None;
     };
   texture WaterbumpTX <string Name=""; >; 
   sampler2D Waterbump=sampler_state 
     {
	Texture = <WaterbumpTX>;
	MagFilter = Linear;	
	MinFilter = Point;
	MipFilter = None;
     };

//--------------
// structs 
//--------------
   struct input
     {
 	float4 Pos:POSITION; 
 	float2 UV:TEXCOORD; 
     };
   struct output
     {
 	float4 OPos:POSITION; 
	float2 Tex0:TEXCOORD0; 
 	float2 Tex1:TEXCOORD1; 
 	float2 Tex2:TEXCOORD2; 
	float3 VP:TEXCOORD3;  
    	float4 RefrProj:TEXCOORD4;
    	float4 ReflProj:TEXCOORD5;
     };

//--------------
// vertex shader
//--------------
   output VS(input IN) 
     {
 	output OUT;
	OUT.OPos=mul(IN.Pos,WorldVP); 
	OUT.Tex0=IN.UV;
	OUT.Tex1=(IN.UV*WaterScale+(time*Speed1));
	OUT.Tex2=(IN.UV*WaterScale*4+(time*Speed2));
	float3 WPos=mul(IN.Pos,World);  
	float3 VP=ViewInv[3].xyz-WPos; 
        OUT.VP=VP;
	OUT.RefrProj=float4(OUT.OPos.x*0.5+0.5*OUT.OPos.w,0.5*OUT.OPos.w-OUT.OPos.y*0.5,OUT.OPos.w,OUT.OPos.w);
  	OUT.ReflProj=float4(OUT.OPos.x*0.5+0.5*OUT.OPos.w,0.5*OUT.OPos.w+OUT.OPos.y*0.5,OUT.OPos.w,OUT.OPos.w);
	return OUT;
    }

//--------------
// pixel shader
//--------------
   float4 ReflRefr(output IN) : COLOR 
    {
    	float4 Distort=tex2D(Waterbump,IN.Tex1);
        Distort=Distort+tex2D(Waterbump,IN.Tex2)-1;
	Distort=Distort*(IN.RefrProj.z*WaterBump);
    	float3 Refraction=tex2Dproj(WaterRefract,IN.RefrProj+Distort)*RefractColor;
	float3 Reflection=tex2Dproj(WaterReflect,IN.ReflProj+Distort)*ReflectColor;

        // New Fresnel Term
        Reflection=Reflection*0.6;
        float3 ViewT = normalize(IN.VP);
        float3 ViewB = normalize(ViewT);
        float Fresnel = pow( abs(ViewB.y), FresPow );
    	float3 Water=lerp(Reflection,Refraction,Fresnel);
	return float4(Water,1);
    } 
   float4 Refr(output IN) : COLOR 
    {
    	float4 Distort=tex2D(Waterbump,IN.Tex1);
        Distort=Distort+tex2D(Waterbump,IN.Tex2)-1;
  	Distort=Distort*(IN.RefrProj.z*WaterBump);
    	float3 Refraction=tex2Dproj(WaterRefract,IN.RefrProj+Distort)*RefractColor;
	return float4(Refraction,1);
    } 

//--------------
// techniques   
//--------------
  technique ReflectRefract
   {
    pass p1
     {		
      VertexShader = compile vs_2_0 VS(); 
      PixelShader  = compile ps_2_0 ReflRefr(); 		
     }
    }
  technique Refract
   {
    pass p1
     {		
      VertexShader = compile vs_2_0 VS(); 
      PixelShader  = compile ps_2_0 Refr(); 		
     }
    }