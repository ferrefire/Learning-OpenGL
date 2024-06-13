#version 330 core
out vec4 FragColor;
in vec3 outColor;

uniform vec4 ourColor;

void main()
{
    //FragColor = ourColor;
    FragColor = vec4(outColor * ourColor.rgb, 1.0);
}