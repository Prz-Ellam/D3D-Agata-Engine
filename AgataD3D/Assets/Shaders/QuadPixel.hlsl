struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
};

Texture2D t_Texture : register(t0);
SamplerState s_Quad : register(s0);

float4 main(PSInput psInput) : SV_TARGET
{
	return t_Texture.Sample(s_Quad, psInput.uv);
}