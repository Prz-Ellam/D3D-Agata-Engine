struct PixelInput {
	float4 pos : SV_POSITION;
	float3 uv : TEXCOORDS;
	float4 blendFactor : BLEND;
};

TextureCube t_CubeMap1 : register(t0);
TextureCube t_CubeMap2 : register(t1);
TextureCube t_CubeMap3 : register(t2);
TextureCube t_CubeMap4 : register(t3);
SamplerState s_SkyboxSampler : register(s0);

float4 main(PixelInput psInput) : SV_TARGET
{
	return t_CubeMap1.Sample(s_SkyboxSampler, psInput.uv) * psInput.blendFactor.x
	+ t_CubeMap2.Sample(s_SkyboxSampler, psInput.uv) * psInput.blendFactor.y
	+ t_CubeMap3.Sample(s_SkyboxSampler, psInput.uv) * psInput.blendFactor.z
	+ t_CubeMap4.Sample(s_SkyboxSampler, psInput.uv) * psInput.blendFactor.w;

}