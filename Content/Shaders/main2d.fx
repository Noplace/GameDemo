//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
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
    float4 ps_color;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float2 Pos : POSITION;
    float2 Tex : TEXCOORD0;
	float4  Col : COLOR0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float4 Col : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	//PS_INPUT output = (PS_INPUT)0;
	//return input;
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(  float4(input.Pos,0,1), world );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
	//output.Pos.x /= vMeshColor.r;
    output.Tex = input.Tex;
	output.Col = input.Col;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	return input.Col*ps_color;// * txDiffuse.Sample( samLinear2, input.Tex );
}


technique11
{
    pass p2 
    {
        SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}
