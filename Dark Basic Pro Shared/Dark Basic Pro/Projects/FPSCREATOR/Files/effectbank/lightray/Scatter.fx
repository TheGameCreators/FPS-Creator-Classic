//====================================================
// Light Scattering
// By EVOLVED
//====================================================

//--------------
// un-tweaks
//--------------
   matrix ViewProjection:ViewProjection;
   matrix ProjMat={0.5,0,0,0.5,0,-0.5,0,0.5,0,0,0.5,0.5,0,0,0,1}; 

//--------------
// tweaks
//--------------
   float3 LightDir={-0.0,-0.5,-0.723};
   int SampleNum=40;
   float Density=1.0f;
   float Decay=0.975f;
   float AlphaAngle=1.0;

//--------------
// Textures
//--------------
   texture BlackTextureTX <string Name=" ";>;
   sampler BlackTexture=sampler_state 
      {
	Texture=<BlackTextureTX>;
   	ADDRESSU = CLAMP;
   	ADDRESSV = CLAMP;
   	ADDRESSW = CLAMP;
      };

//--------------
// structs 
//--------------
   struct InPut
     {
 	float4 Pos:POSITION;
 	float2 UV:TEXCOORD; 
     };
   struct OutPut
     {
	float4 OPos:POSITION; 
 	float2 Tex:TEXCOORD0;
     };

//--------------
// vertex shader
//--------------
   OutPut VS(InPut IN) 
     {
 	OutPut OUT;
	OUT.OPos=IN.Pos; 
 	OUT.Tex=IN.UV;
	return OUT;
    }

//--------------
// pixel shader
//--------------
   float4 PS(OutPut IN) : COLOR
     {
	float4 ScreenToLight=mul(ProjMat,mul(-LightDir,ViewProjection));
	float2 DeltaTex=((ScreenToLight.xy/ScreenToLight.z)-IN.Tex)*sign(ScreenToLight.z);
	float length=length(DeltaTex);DeltaTex /=length;
	length=min(length,Density);DeltaTex *=length;
	float2 NewUv=IN.Tex;
	float3 Scatter=0;
	float FallOff=1.0;
	DeltaTex /=SampleNum;
	for (int i=0; i < SampleNum; i++)
	 {		
	  NewUv +=DeltaTex;
	  Scatter +=(tex2D(BlackTexture,NewUv)*AlphaAngle)*FallOff;
	  FallOff *=Decay;
	 }
	Scatter /=SampleNum;	
	return float4(Scatter,1);
     }

//--------------
// techniques   
//--------------
    technique Scatter
      {
 	pass p1
      {		
 	VertexShader = compile vs_3_0 VS(); 
 	PixelShader  = compile ps_3_0 PS();
      }
      }