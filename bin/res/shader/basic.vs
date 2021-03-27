#version 330 core

layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normals;

out vec2 f_tex_coord;
out vec3 f_normals;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertex_pos, 1.0);
	frag_pos = vec3(model * vec4(vertex_pos, 1.0));
	f_tex_coord = tex_coord;
	// Calculating the normal matrix should be done on CPU side for fastness
	// but since its christmas, im gunna let you slide.
	f_normals = mat3(transpose(inverse(model))) * normals;
}