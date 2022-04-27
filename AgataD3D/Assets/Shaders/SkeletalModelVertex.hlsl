struct VSInput {
	float3 pos : POSITION;
	float2 uv : TEXCOORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 joint : JOINT;
	float4 weight : WEIGHT;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float3 normal : NORMAL;
	float3 toLightVector : LIGHTVECTOR;
	float3 toCameraVector : CAMERAVECTOR;
	float4 lightColour : LIGHTCOLOUR;
};


cbuffer VS_CB : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float4 c_CameraPosition;
	float4 c_LightPosition;
	float4 c_LightColour;
	matrix c_Joints[58];
}

PSInput main(VSInput input) {

	PSInput output;
	matrix boneTransform = { { 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 } };
	boneTransform = c_Joints[input.joint.x] * input.weight.x;
	boneTransform += c_Joints[input.joint.y] * input.weight.y;
	boneTransform += c_Joints[input.joint.z] * input.weight.z;
	boneTransform += c_Joints[input.joint.w] * input.weight.w;

	float4 posePos = mul(float4(input.pos, 1.0f), boneTransform);

	float4 worldPos = mul(posePos, c_Model);
	output.pos = worldPos;
	output.pos = mul(output.pos, c_View);
	output.pos = mul(output.pos, c_Projection);
	output.uv = input.uv;

	output.normal = input.normal;
	output.lightColour = c_LightColour;

	float3 T = normalize(mul(c_Model, float4(input.tangent, 0.0f)).xyz);
	float3 N = normalize(mul(c_Model, float4(input.normal, 0.0f)).xyz);
	T = normalize(T - dot(T, N) * N);
	float3 B = normalize(mul(c_Model, float4(input.bitangent, 0.0f)).xyz);
	float3x3 TBN = transpose(float3x3(T, B, N));

	output.toLightVector = mul(normalize(c_LightPosition - worldPos.xyz), TBN);
	output.toCameraVector = mul(normalize(c_CameraPosition - worldPos.xyz), TBN);

	return output;

}