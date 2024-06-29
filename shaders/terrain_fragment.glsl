#version 460 core
in vec2 UV;
in vec3 Normal;
in vec3 FragmentPosition;
in vec4 Color;

out vec4 oFragmentColor;

uniform sampler2D texture;
uniform float time;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform float near;
uniform float far;

//float random (vec2 st)
//{
//    return fract(sin(dot(st.xy, vec2(12d.9898,78.233))) * 43758.5453123);
//}

float GetDepth()
{
    float depth = gl_FragCoord.z;

    depth = depth * 2.0 - 1.0;
    depth = (2.0 * near * far) / (far + near - depth * (far - near));
    depth = depth / far;

    return (depth);
}

void main()
{
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 normal = normalize(Normal);

	float diffuse = dot(normal, lightDirection);
	diffuse = diffuse * 0.5f + 0.5f;
    diffuse = diffuse * diffuse;
    //vec3 diff = vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specularColor = vec3(0.5 * specular);

    oFragmentColor = mix(vec4(Color.xyz * (vec3(0.25) + diffuse + specularColor), 1.0), vec4(1.0), GetDepth());
}