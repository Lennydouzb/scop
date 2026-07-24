#version 410 core


in vec3 aNormal;
in vec3 aColor;
in vec3 fragPos;

out vec4 fragmentColor;
layout (location = 4) in vec3 ka;
layout (location = 5) vec3 kd;
layout (location = 6) vec3 ks;
layout (location = 7) float d;
layout (location = 8) float ns;
void	main()
{
	vec3 light = normalize(vec3(0.4,0.6, 1.0));
	// dot returns 1 if in front of light, 0 if not,
	vec3 diffuse = max(dot(aNormal, light), 0.0) * (kd * aColor);
	vec3 ambiant = (ka * aColor);
	vec3 specular = pow(max(dot(normalize(-fragPos), reflect(-light, aNormal)), 0.0), ns) * ks;
	//apply the light effect
	fragmentColor = vec4(diffuse + ambiant + specular, d);
}
