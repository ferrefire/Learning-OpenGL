#version 330 core
out vec4 FragColor;
in vec2 UV;
in vec3 Norm;
in vec3 FragPos;
in vec3 ranCol;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAmount;
uniform float time;
//uniform int countRoot;
uniform vec4 colMult = vec4(1.0f);
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float near;
uniform float far;

//float random (vec2 st)
//{
//    return fract(sin(dot(st.xy, vec2(12d.9898,78.233))) * 43758.5453123);
//}

float getDepth()
{
    float depth = gl_FragCoord.z;
    depth = depth * 2.0 - 1.0;
    depth = (2.0 * near * far) / (far + near - depth * (far - near));
    depth = depth / far;
    return (depth);
}

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Norm);

	float diff = dot(norm, lightDir);
	diff = diff * 0.5f + 0.5f;
    diff = diff * diff;
    //vec3 diff = vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(0.75 * spec);

    FragColor = mix(vec4(ranCol * (vec3(0.25) + diff + specular), 1.0), vec4(1.0), getDepth());
}