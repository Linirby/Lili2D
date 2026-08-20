struct VSInput {
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
    float material_id : TEXCOORD1;
    float4 color : COLOR;
};

struct VSOutput {
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    nointerpolation uint material_id : TEXCOORD1;
    float4 color : COLOR;
};

cbuffer UniformBlock : register(b0, space1) {
    float3x3 mat;
    float4 color_tint;
    float4 uv_bounds;
    float layer;
    float time;
    float2 padding;
};

VSOutput
main(VSInput input) {
    VSOutput output;

    float3 pos2d = mul(mat, float3(input.pos.x, input.pos.y, 1.0));
    float2 uv_min = uv_bounds.xy;
    float2 uv_max = uv_bounds.zw;

    output.pos = float4(pos2d.xy, input.pos.z + layer, 1.0);
    output.uv = uv_min + input.uv * (uv_max - uv_min);
    output.material_id = (uint)(input.material_id + 0.5);
    output.color = color_tint * input.color;
    return output;
}
