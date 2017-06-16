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
	float3 Pos : POSITION;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 wPos : POSITION;
};

VS_OUT VS_main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.wPos = mul( float4( input.Pos, 1.0f ), world );
	output.Pos = mul( float4( input.Pos, 1.0f ), mul( world, mul( view, projection ) ) );
	
	return output;
}


//########################################################################################
//##								 PIXEL SHADER										##
//########################################################################################

float4 PS_main( VS_OUT input ) : SV_Target
{
	float4 outputColor = input.wPos;

	return outputColor;
};