#version 330 core
out vec4 FragColor;
//in vec3 outColor;
in vec2 UV;

//uniform vec4 ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAmount;
uniform vec4 colMult;

void main()
{
    //FragColor = colMult * 2;
    //FragColor = vec4(outColor * ourColor.rgb, 1.0);
    FragColor = mix(texture(texture1, UV), texture(texture2, UV), mixAmount) * colMult * 2;
}