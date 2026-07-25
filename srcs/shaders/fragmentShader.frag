#version 330 core

in vec3 aNormal;
in vec3 aColor;
in vec3 fragPos;

out vec4 fragmentColor;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float d;
uniform float ns;

void main()
{
    vec3 light = normalize(vec3(0.4, 0.6, 1.0));
    vec3 ambiant = max(ka, vec3(0.15)) * aColor;
    float diffImpact = max(dot(aNormal, light), 0.0);
    vec3 diffuse = diffImpact * (kd * aColor);
    
    vec3 specular = vec3(0.0);
	//only if enlighted
    if (diffImpact > 0.0 && ns > 0.0)
    {
        float specImpact = max(dot(normalize(-fragPos), reflect(-light, aNormal)), 0.0);
        specular = pow(specImpact, ns) * ks;
    }
    
    fragmentColor = vec4(diffuse + ambiant + specular, d);
}
