struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float tiling : TILING;
};

SamplerState terrainSampler : register(s0);

Texture2D t_RedDiffuse : register(t0);
Texture2D t_GreenDiffuse : register(t1);
Texture2D t_BlueDiffuse : register(t2);
Texture2D t_BlackDiffuse : register(t3);
Texture2D t_BlendMap : register(t4);

float3 blendTexture(float4 blendMap, Texture2D background, Texture2D red, Texture2D green, Texture2D blue, float2 uv) {

	float blendMapBlack = clamp(1 - (blendMap.x + blendMap.y + blendMap.z), 0.0f, 1.0f);

	float4 redColour = t_RedDiffuse.Sample(terrainSampler, uv);
	float4 greenColour = t_GreenDiffuse.Sample(terrainSampler, uv);
	float4 blueColour = t_BlueDiffuse.Sample(terrainSampler, uv);
	float4 blackColour = t_BlackDiffuse.Sample(terrainSampler, uv);
	
	float4 redGray = redColour.x * 0.299 + redColour.y * 0.587 + redColour.z * 0.114;
	float4 greenGray = greenColour.x * 0.299 + greenColour.y * 0.587 + greenColour.z * 0.114;
	float4 blueGray = blueColour.x * 0.299 + blueColour.y * 0.587 + blueColour.z * 0.114;
	float4 blackGray = blackColour.x * 0.299 + blackColour.y * 0.587 + blackColour.z * 0.114;

	float depth = 0.2;
	float ma = max(redGray + blendMap.r, max(greenGray + blendMap.g, 
			max(blueGray + blendMap.b, blackGray + blendMapBlack))) - depth;

	float b1 = max(redGray + blendMap.x - ma, 0);
	float b2 = max(greenGray + blendMap.y - ma, 0);
	float b3 = max(blueGray + blendMap.z - ma, 0);
	float b4 = max(blackGray + blendMapBlack - ma, 0);

	float3 blackChannel = blackColour.xyz * b4;
	float3 redChannel = redColour.xyz * b1;
	float3 greenChannel = greenColour.xyz * b2;
	float3 blueChannel = blueColour.xyz * b3;

	return (blackChannel + redChannel + greenChannel + blueChannel) / (b1 + b2 + b3 + b4);

}

float4 main(PSInput input) : SV_TARGET{

	float4 blend = t_BlendMap.Sample(terrainSampler, input.uv);
	float2 tileUV = input.uv * input.tiling;

	return float4(blendTexture(blend, t_BlackDiffuse, t_RedDiffuse, t_GreenDiffuse, t_BlueDiffuse, tileUV), 1.0f);

}