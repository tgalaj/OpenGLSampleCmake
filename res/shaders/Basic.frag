#version 430

in vec3 o_position;
in vec3 o_normal;
in vec2 o_texcoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D tex_sampler;
uniform vec3 cam_pos;

void main()
{   
	vec3 n = normalize(o_normal);
	vec3 frag_pos = normalize(cam_pos - o_position);
	
	vec3 finalColor = texture(tex_sampler, o_texcoord).xyz;
	finalColor *= max(0.0f, dot(n, frag_pos));
	
	fragColor = vec4(finalColor, 1.0f);
}