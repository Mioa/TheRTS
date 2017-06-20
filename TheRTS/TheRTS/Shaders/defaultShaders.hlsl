//########################################################################################
//##								 VERTEX SHADER										##
//########################################################################################

cbuffer objectCB : register( b0 )
{
	float4x4 view;
	float4x4 projection;
};

cbuffer objectCB : register( b1 )
{
	float4x4 world;
};

struct VS_IN
{
	float3 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float4 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
	float4 wPos		: POSITION;
};

VS_OUT VS_main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.wPos		= mul( float4( input.Pos, 1.0f ), world );
	output.Pos		= mul( output.wPos, mul( view, projection ) );
	output.Normal	= mul( float4( input.Normal, 0.0f ), world );
	output.UV		= input.UV;
	
	return output;
}


//########################################################################################
//##								 PIXEL SHADER										##
//########################################################################################

Texture2D albedoTex : register( t0 );

SamplerState linearSamp : register( s0 );

float4 PS_main( VS_OUT input ) : SV_Target
{
	float4 dirLight = float4( 0.5f, 1.0f, 0.5f, 0.0f );
	float4 outputColor = dot(normalize(input.Normal), normalize(dirLight)) * albedoTex.Sample( linearSamp, input.UV );

	return outputColor;
}