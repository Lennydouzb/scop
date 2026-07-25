#version 330 core

in vec3 aNormal;
in vec3 aColor;
in vec3 fragPos;
in vec2 tx;
out vec4 fragmentColor;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float d;
uniform float ns;
uniform sampler2D tex;
uniform int isTextured;

void main()
{
	//noise to create the "subtle shade of grey"
	float variation = ((gl_PrimitiveID % 2) == 0) ? 0.05 : -0.05;
	vec3 theColor = aColor;
	if (aColor.r < 0.0)
		theColor = vec3(0.75, 0.75, 0.75);
    vec3 light = normalize(vec3(0.4, 0.6, 1.0));
    vec3 ambiant = max(ka, vec3(0.15)) * theColor;
    float diffImpact = max(dot(aNormal, light) + variation, 0.0);
    vec3 diffuse = diffImpact * (kd * theColor);
    
    vec3 specular = vec3(0.0);
	//only if enlighted
    if (diffImpact > 0.0 && ns > 0.0)
    {
        float specImpact = max(dot(normalize(-fragPos), reflect(-light, aNormal)), 0.0);
        specular = pow(specImpact, ns) * ks;
    }
	vec4 texColor;
    if (isTextured == 1)
		texColor = texture(tex, tx);
	else
		texColor = vec4(1.0, 1.0, 1.0 ,1.0);

	fragmentColor = vec4((diffuse + ambiant + specular) * texColor.rgb, d * texColor.a);
}
