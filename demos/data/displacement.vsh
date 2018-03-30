#version 400 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_texcoord;

out vec3 v_pos;      // pixel view space position
out vec2 v_texcoord; // texture coords
out vec3 v_normal;   // normal
out vec3 v_tangent;

// out vec3 v_binormal; // binormal (for TBN basis calc)
// out vec4 v_screen_space;

uniform float u_texcoord_rotation;
uniform sampler2D us_displacement;

void main()
{
	v_pos = a_position;
	v_normal = a_normal;
	v_tangent = a_tangent;

	float t = u_texcoord_rotation;
	mat2 uv_rot = mat2(
		vec2(cos(t), sin(t)),
		vec2(-sin(t), cos(t))
	);

	const float scale = 4.0;
	v_texcoord = uv_rot * ((a_texcoord.xy * scale) - (0.5 * scale));
	v_texcoord += vec2(0.5 * scale);

	// v_pos = a_position + (texture(us_displacement, v_texcoord).x - 0.35) * v_displacement_weight * a_normal;
}
