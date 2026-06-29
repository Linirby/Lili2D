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

layout(set = 1, binding = 1) uniform TextUB {
	float speed;
	float time;
} custom;

void main() {
	vec3 pos2d = ubo.matrix * vec3(in_pos.x, in_pos.y, 1.0);
	gl_Position = vec4(pos2d.xy, in_pos.z + ubo.layer, 1.0);
	
	v_uv = in_uv;
	v_material_id = uint(in_material_id + 0.5);
	
	// Calculate dynamic rainbow from left to right using in_pos.x and time
	float hue = custom.time * custom.speed - in_pos.x * 0.02;
	vec3 rainbow = 0.5 + 0.5 * cos(hue + vec3(0.0, 2.094, 4.188));
	v_color = vec4(rainbow, 1.0) * ubo.color_tint;
}
