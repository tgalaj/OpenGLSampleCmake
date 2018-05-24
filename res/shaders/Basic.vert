#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;

out vec3 o_position;
out vec3 o_normal;
out vec2 o_texcoord;
	
uniform mat4 world;
uniform mat4 viewProj;
uniform mat3 normalMatrix;
	
void main()
{
	o_position = vec3(world * vec4(position, 1.0f));
    o_normal   = normalMatrix * normal;
    o_texcoord = texcoord.xy;
	
    gl_Position = viewProj * world * vec4(position, 1.0f);
}