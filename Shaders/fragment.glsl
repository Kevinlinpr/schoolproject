#version 450

in vec3 FragNormal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform vec3 ambientColor;
uniform float ambientStrength;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{   
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos - FragPos); 

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 ambient = ambientStrength * ambientColor;
    
    vec3 result = (ambient + diffuse) * vec3(0.2f, 0.2f, 1.0f);
    //color = vec4(texture(texture_diffuse1, TexCoord0));
	color = vec4(result, 1.0f);
}
