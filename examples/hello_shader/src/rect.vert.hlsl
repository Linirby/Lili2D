struct VSInput {
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
    float material_id : TEXCOORD1;
};

struct VSOutput {
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    nointerpolation uint material_id : TEXCOORD1;
    float4 color : COLOR0;
};

cbuffer UniformBlock : register(b0, space1) {
    float3x3 mat;
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
        sin(custom_time * speed + input.pos.y * frequency) * amplitude;
    float3 pos2d =
        mul(mat, float3(input.pos.x + wave_offset, input.pos.y, 1.0));

    output.pos = float4(pos2d.xy, input.pos.z + layer, 1.0);
    output.uv = input.uv;
    output.material_id = uint(input.material_id + 0.5);
    output.color = color_tint;
    return output;
}