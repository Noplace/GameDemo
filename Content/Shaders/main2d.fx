//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
Texture2DArray font_textures : register( t1 );
SamplerState samLinear : register( s0 );

SamplerState samLinear2
{
Filter = MIN_MAG_MIP_LINEAR;
AddressU = Clamp;
AddressV = Clamp;
AddressW = Wrap;
ComparisonFunc = NEVER;
MinLOD = 0;
//MaxLOD = MAX;
};
BlendState NoBlend
{ 
    BlendEnable[0] = False;
};


cbuffer cbCamera : register( b0 )
{
    matrix View;
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
    matrix world;
	bool enable_texture;
    float4 ps_color;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	uint page : BLENDINDICES0;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	uint page : BLENDINDICES0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	//PS_INPUT output = (PS_INPUT)0;
	//return input;
    PS_INPUT output = (PS_INPUT)0;
    output.pos = mul(  float4(input.pos,1), world );
    output.pos = mul( output.pos, View );
    output.pos = mul( output.pos, Projection );
	//output.Pos.x -= output.Pos.z;
    output.uv = input.uv;
	output.color = input.color;
	output.page = input.page;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	//if (enable_texture == true)
	//	return input.Col*ps_color * txDiffuse.Sample( samLinear2, input.Tex );
	//else
		return input.color*ps_color;
}

float4 PSTex( PS_INPUT input) : SV_Target
{
	return input.color*ps_color * txDiffuse.Sample( samLinear2, input.uv );
}

float4 PSFont( PS_INPUT input) : SV_Target
{
    float4 pixel = 0;
	uint index = 0;
	pixel = font_textures.Sample( samLinear2, float3(input.uv,input.page) );

	pixel.a = pixel.a * ps_color;
	return pixel * input.color;
}



technique11
{
    pass p2 
    {
        SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}
