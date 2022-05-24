struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float3 normal : NORMAL;
	float3 toLightVector : LIGHTVECTOR;
	float3 toCameraVector : CAMERAVECTOR;
	float4 lightColour : LIGHTCOLOUR;
};

cbuffer PS_CB : register(b1) {
	float4 c_AmbientMaterial;
	float4 c_DiffuseMaterial;
	float4 c_SpecularMaterial;
	float c_Shininess;
}

Texture2D tex : register(t0);
Texture2D t_NormalMap : register(t1);
Texture2D t_SpecularMap : register(t2);
SamplerState samplerX : register(s0);

float4 main(PSInput input) : SV_TARGET {

	// Normal Map
	float3 unitNormal = 2.0f * t_NormalMap.Sample(samplerX, input.uv).xyz - 1.0f;
	unitNormal = normalize(unitNormal);


	float4 texColour = tex.Sample(samplerX, input.uv);


	// Ambient Light
	float3 ambient = input.lightColour.xyz * c_AmbientMaterial.xyz;


	//float3 unitNormal = normalize(input.normal);

	// Diffuse Light
	float diffuseScalar = dot(unitNormal, normalize(input.toLightVector));
	diffuseScalar = max(diffuseScalar, 0.0f);
	float3 diffuse = diffuseScalar * c_DiffuseMaterial * input.lightColour;
	float4 diffuseColour = tex.Sample(samplerX, input.uv);


	// Specular Light
	float3 lightDirection = -normalize(input.toLightVector);
	float3 reflectLightDir = reflect(lightDirection, unitNormal);
	float specularScalar = dot(reflectLightDir, normalize(input.toCameraVector));
	specularScalar = max(specularScalar, 0.0f);
	specularScalar = pow(specularScalar, c_Shininess);
	float4 specularMap = t_SpecularMap.Sample(samplerX, input.uv);
	float4 specular = specularScalar * c_SpecularMaterial * input.lightColour * specularMap;


	clip(texColour.a < 0.5f ? -1 : 1); // Discard in GLSL

	return diffuseColour * float4(ambient + diffuse + specular, 1.0f);


}