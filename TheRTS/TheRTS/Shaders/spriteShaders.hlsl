//########################################################################################
//##								 VERTEX SHADER										##
//########################################################################################

cbuffer spriteCB : register( b0 )
{
	float4 spritePos[200];
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION; // Vertex ID
	float2 UV		: TEXCOORD;
};

struct Vertex
{
	float4 Pos;
	float2 UV;
};

//static const Vertex vertices[4] = {
//	-1.0f, 1.0f, 0.0f, 1.0f,
//	0.0f, 0.0f,
//	1.0f, 1.0f, 0.0f, 1.0f,
//	1.0f, 0.0f,
//	-1.0f, -1.0f, 0.0f, 1.0f,
//	0.0f, 1.0f,
//	1.0f, -1.0f, 0.0f, 1.0f,
//	1.0f, 1.0f,
//};

static const Vertex vertices[6] = {
	-1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f
};

VS_OUT VS_main( uint id : SV_VertexID )
{
	VS_OUT output = (VS_OUT)0;

	float windowWidth	= 960;
	float windowHeight	= 600;

	int vertexID = id % 6;
	int spriteID = id / 6;

	float4 sPos = spritePos[spriteID];
	sPos.x /= windowWidth;
	sPos.y /= windowHeight;
	sPos.z /= windowWidth;
	sPos.w /= windowHeight;
	sPos *= 2.0f;

	output.Pos = vertices[vertexID].Pos;
	output.Pos.x += sPos.x;
	output.Pos.y -= sPos.y;

	if( vertexID == 1 )
		output.Pos.x += sPos.z;
	if( vertexID == 2 || vertexID == 3 )
	{
		output.Pos.x += sPos.z;
		output.Pos.y -= sPos.w;
	}
	if( vertexID == 4 )
		output.Pos.y -= sPos.w;

	output.UV = vertices[id % 6].UV;

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