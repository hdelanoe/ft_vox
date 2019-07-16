#version 330 core

out vec4 FragColor;

in vec2 Text;
in vec3 Norm;
in vec3 FragPos;

in vec2 tileUV;
in vec2 tileOffset;
in float visibility;

uniform sampler2D	text;

uniform vec3		playerPos;
uniform vec3		lightPos;
uniform vec3		viewDir;
uniform bool		underWater;

void main()
{
	vec3 lightColor = vec3(1.0f, 0.64f, 0);
	float ambientStrength = 0.8f;
	vec2 tileSize = vec2(1.0f / 16.0f, 0.0625f);

	vec2 TexCoord = tileOffset + (tileSize * fract(tileUV));
	vec4 texColor = texture(text, TexCoord);
	if (texColor.a < 0.1)
		discard;
	vec3 ambient = ambientStrength * vec3(texColor);
	if (underWater)
		ambient *= vec3(0.1, 0.68, 0.94);

	float diffuseStrenght = 0.15f;
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(Norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor * diffuseStrenght;

	float specularStrength = 0.5f;
	vec3 reflectDir = reflect(-lightDir, -Norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = specularStrength * spec * lightColor;
	vec3 result = (ambient + diffuse + specular);

	FragColor = vec4(result, visibility);
}
