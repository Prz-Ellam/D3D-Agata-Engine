struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float3 normal : NORMAL;
	float3 toLightVector : LIGHTVECTOR;
	float3 toCameraVector : CAMERAVECTOR;
	float4 lightColour : LIGHTCOLOUR;
};

Texture2D t_DiffuseTexture : register(t0);
//Texture2D t_NormalMap : register(t1);
SamplerState s_Sampler : register(s0);

float4 main(PSInput input) : SV_TARGET{

	return t_DiffuseTexture.Sample(s_Sampler, input.uv);


}