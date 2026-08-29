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

cbuffer RectUB : register(b1, space1) {
    float custom_time;
    float amplitude;
    float frequency;
    float speed;
};

VSOutput
main(VSInput input) {
    VSOutput output;
    float wave_offset =
        sin(custom_time * speed + input.in_pos.y * frequency) * amplitude;

    float3 pos2d =
        mul(u_matrix,
            float3(input.in_pos.x + wave_offset, input.in_pos.y, 1.0));
    output.pos = float4(pos2d.xy, input.in_pos.z + layer, 1.0);

    output.uv = input.in_uv;
    output.material_id = (uint)(input.in_material_id + 0.5);
    output.color = color_tint;
    return output;
}
