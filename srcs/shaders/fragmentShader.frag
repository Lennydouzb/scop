#version 330 core


in vec3 aNormal;
in vec3 aColor;
in vec3 pos;

out vec4 fragmentColor;
in vec3 ka;
in vec3 kd;
in vec3 ks;
in float d;
void	main()
{
	vec3 light = normalize(vec3(0.4,0.6, 1.0));
	// dot returns 1 if in front of light, 0 if not,
	vec3 diffuse = max(dot(aNormal, light), 0.0) * (kd * aColor);
	vec3 ambiant = (ka * aColor);
	vec3 specular = pow(max(dot(normalize(-pos), reflect(-light, aNormal)), 0.0), ns) * ks;
	//apply the light effect
	fragmentColor = vec4(diffuse + ambiant + specular, d);
}
