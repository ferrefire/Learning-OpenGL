#version 330 core
out vec4 FragColor;
//in vec3 outColor;
in vec2 UV;

//uniform vec4 ourColor;
uniform sampler2D mainTexture;

void main()
{
    //FragColor = ourColor;
    //FragColor = vec4(outColor * ourColor.rgb, 1.0);
    FragColor = texture(mainTexture, UV);
}