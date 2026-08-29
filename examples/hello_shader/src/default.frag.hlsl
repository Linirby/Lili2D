struct PSInput {
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    nointerpolation uint material_id : TEXCOORD1;
    float4 color : COLOR0;
};

Texture2D u_albedo_map : register(t0, space2);
SamplerState u_sampler : register(s0, space2);

float4
main(PSInput input)
    : SV_Target0 {
    float4 tex_color = u_albedo_map.Sample(u_sampler, input.uv);
    return tex_color * input.color;
}
