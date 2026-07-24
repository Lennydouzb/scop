#version 330 core


//this is the 0 from  glvertexAttribPointer of GPU.cpp
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 aTexture;
uniform mat4 mvp;
uniform mat4 rotation;
out vec3 aColor;
out vec3 aNormal;
out vec3 fragPos;

void	main()
{
	gl_Position = mvp * vec4(pos, 1.0);
	aNormal = normalize(mat3(rotation) * normal);
	fragPos = vec3(rotation * vec4(pos, 1.0));
	aColor = color;
}
