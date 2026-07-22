#version 330 core


in vec3 aNormal;
in vec3 aColor;


out vec4 fragmentColor;
void	main()
{
	vec3 light = normalize(vec3(0.4,0.6, 1.0));
	// dot returns 1 if in front of light, 0 if not,
	//max is to make 0.14 the minimum ligh so not enlighted poart are not invisble
	float diff = max(dot(normalize(aNormal), light), 0.15);
	//apply the light effect
	fragmentColor = vec4(aColor * diff, 1.0);
}
