#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in float in_material_id;

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

layout(set = 1, binding = 1) uniform RectUB {
	float time;
	float amplitude;
	float frequency;
	float speed;
} custom;

void main() {
	float wave_offset = sin(
		custom.time * custom.speed + in_pos.y * custom.frequency
	) * custom.amplitude;
	
	vec3 pos2d = ubo.matrix * vec3(in_pos.x + wave_offset, in_pos.y, 1.0);
	gl_Position = vec4(pos2d.xy, in_pos.z + ubo.layer, 1.0);
	
	v_uv = in_uv;
	v_material_id = uint(in_material_id + 0.5);
	v_color = ubo.color_tint;
}
