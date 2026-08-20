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

cbuffer TextUB : register(b1, space1) {
    float speed;
    float custom_time;
};

VSOutput main(VSInput input) {
    VSOutput output;
    float3 pos2d = mul(u_matrix, float3(input.in_pos.x, input.in_pos.y, 1.0));
    output.pos = float4(pos2d.xy, input.in_pos.z + layer, 1.0);
    
    output.uv = input.in_uv;
    output.material_id = (uint)(input.in_material_id + 0.5);
    
    float hue = custom_time * speed - input.in_pos.x * 0.02;
    float3 rainbow = 0.5 + 0.5 * cos(hue + float3(0.0, 2.094, 4.188));
    output.color = float4(rainbow, 1.0) * color_tint;
    return output;
}
