#pragma once

namespace lili::shaders {

/// @brief Default 2D world vertex shader in HLSL.
inline constexpr const char* world_2d_vert_hlsl = R"hlsl(
struct VSInput {
    float3 in_pos : POSITION;
    float2 in_uv : TEXCOORD0;
    float in_material_id : TEXCOORD1;
    float4 in_color : COLOR0;
};

struct VSOutput {
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    nointerpolation uint material_id : TEXCOORD1;
    float4 color : COLOR0;
};

cbuffer UniformBlock : register(b0, space1) {
    float3x3 u_matrix;
    float4 color_tint;
    float4 uv_bounds;
    float layer;
    float time;
    float2 padding;
};

VSOutput main(VSInput input) {
    VSOutput output;
    float3 pos2d = mul(u_matrix, float3(input.in_pos.x, input.in_pos.y, 1.0));
    output.pos = float4(pos2d.xy, input.in_pos.z + layer, 1.0);
    float2 uv_min = uv_bounds.xy;
    float2 uv_max = uv_bounds.zw;
    output.uv = uv_min + input.in_uv * (uv_max - uv_min);
    output.material_id = (uint)(input.in_material_id + 0.5);
    output.color = color_tint * input.in_color;
    return output;
}
)hlsl";

/// @brief Default 2D world fragment shader in HLSL.
inline constexpr const char* world_2d_frag_hlsl = R"hlsl(
struct PSInput {
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    nointerpolation uint material_id : TEXCOORD1;
    float4 color : COLOR0;
};

Texture2D u_albedo_map : register(t0, space2);
SamplerState u_sampler : register(s0, space2);

float4 main(PSInput input) : SV_Target0 {
    float4 tex_color = u_albedo_map.Sample(u_sampler, input.uv);
    return tex_color * input.color;
}
)hlsl";

}  // namespace lili::shaders
