#version 450

layout(location = 0) in vec2 v_uv;
layout(location = 3) in vec4 v_color;

layout(set = 2, binding = 0) uniform sampler2D u_albedo_map;

layout(location = 0) out vec4 out_color;

void main() {
	vec4 tex_color = texture(u_albedo_map, v_uv);
	
	out_color = tex_color * v_color;
}
