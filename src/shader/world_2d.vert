#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in float in_material_id;
layout(location = 4) in vec4 in_color;

layout(location = 0) out vec2 v_uv;
layout(location = 2) flat out uint v_material_id;
layout(location = 3) out vec4 v_color;

layout(set = 1, binding = 0) uniform UniformBlock {
	mat3 matrix;
	vec4 color_tint;
	vec4 uv_bounds;
	float layer;
	float time;
	float padding[2];
} ubo;

void main() {
	vec3 pos2d = ubo.matrix * vec3(in_pos.x, in_pos.y, 1.0);
	gl_Position = vec4(pos2d.xy, in_pos.z + ubo.layer, 1.0);
	vec2 uv_min = ubo.uv_bounds.xy;
	vec2 uv_max = ubo.uv_bounds.zw;
	v_uv = uv_min + in_uv * (uv_max - uv_min);
	v_material_id = uint(in_material_id + 0.5);
	v_color = ubo.color_tint * in_color;
}
