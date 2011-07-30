//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D textPage1 : register( t0 );
Texture2D textPage2 : register( t1 );
Texture2D textPage3 : register( t2 );
SamplerState samLinear
{
Filter = MIN_MAG_MIP_LINEAR;
AddressU = Clamp;
AddressV = Clamp;
AddressW = Wrap;
ComparisonFunc = NEVER;
MinLOD = 0;
//MaxLOD = MAX;
};

cbuffer cbCamera : register( b0 )
{
    matrix View;
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
    matrix World;
    float4 vMeshColor;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float2 Pos : POSITION;
    float2 Tex : TEXCOORD0;
	float4  Col : COLOR0;
	uint Channel : BLENDINDICES0;
	uint Page : BLENDINDICES1;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float4 Col : COLOR0;
	uint Channel : BLENDINDICES0;
	uint Page : BLENDINDICES1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    //output.Pos = mul(  float4(input.Pos,0,1), World );
    output.Pos = mul( float4(input.Pos,0,1), View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex;
	output.Col = input.Col; 
	output.Channel = input.Channel;
	output.Page = input.Page;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{

    float4 pixel = 0;
	
	//if (input.Page = 0) {
		pixel = textPage1.Sample( samLinear, input.Tex ) * (1-input.Page)  ;
	//}

    //if (input.Page = 1) {
		pixel += textPage2.Sample( samLinear, input.Tex )* (input.Page) ;
    //}

    // Are we rendering a colored image, or 
    // a character from only one of the channels
    if( dot(vector(1,1,1,1), input.Channel) )
    {
        // Get the pixel value
		float val = dot(pixel, input.Channel);
		
        pixel.rgb = 1;
        pixel.a   = val;
    }

	return pixel * input.Col;

	//return input.Col * textPage1.Sample( samLinear, input.Tex ) ;
}
