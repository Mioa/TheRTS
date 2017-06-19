//########################################################################################
//##								 VERTEX SHADER										##
//########################################################################################

cbuffer spriteCB : register( b0 )
{
	float4 position[200];
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION; // Vertex ID
	float2 UV		: TEXCOORD;
};

VS_OUT VS_main( uint id : SV_VertexID )
{
	VS_OUT output = (VS_OUT)0;

	output.Pos	= float4( 0.5f, 0.5f, 0.5f, 1.0f );
	output.UV	= float2( 0.0f, 0.0f );
	
	return output;
}


//########################################################################################
//##								 PIXEL SHADER										##
//########################################################################################

Texture2D albedoTex : register( t0 );

SamplerState linearSamp : register( s0 );

float4 PS_main( VS_OUT input ) : SV_Target
{
	float4 outputColor = albedoTex.Sample( linearSamp, input.UV );

	return outputColor;
}