//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
//Texture2D textPage1 : register( t0 );
//Texture2D textPage2 : register( t1 );
//Texture2D textPage3 : register( t2 );

Texture2DArray textures : register( t0 );

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

cbuffer cbMisc : register ( b1 )
{
	matrix worldTransform;
	float globalAlpha;
	float4 globalColor;
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
	float4 pos = float4(input.Pos,0,1);
    pos = mul(  pos, worldTransform );
    pos = mul( pos, View );
    output.Pos = mul( pos, Projection );
    output.Tex = input.Tex;
	output.Col = input.Col; 
	output.Channel = input.Channel;
	output.Page = input.Page;
    return output;
}
/*
[maxvertexcount(6)]
void GS(triangle PS_INPUT input[3], inout TriangleStream<PS_INPUT> stream)
{
	stream.Append( input[0] );
	stream.Append( input[1] );
	stream.Append( input[2] );
	stream.RestartStrip();

}*/


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{

    float4 pixel = 0;
	uint index = 0;
	pixel = textures.Sample( samLinear, float3(input.Tex,input.Page) );


	/*
    // Are we rendering a colored image, or 
    // a character from only one of the channels
    if( dot(vector(1,1,1,1), input.Channel) )
    {
        // Get the pixel value
		float val = dot(pixel, input.Channel);
		
        pixel.rgb = 1;
        pixel.a   = val;
    }
	*/

	//pixel.rgb = globalColor.rgb;
	pixel.a = pixel.a * globalAlpha;
	return pixel * input.Col;
}
