#version 330 core
out vec4 FragColor;
//in vec3 outColor;
in vec2 UV;
in vec3 Norm;
in vec3 ranCol;

//uniform vec4 ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAmount;
uniform float time;
uniform int id;
uniform vec4 colMult = vec4(1.0f);

//float random (vec2 st)
//{
//    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
//}

void main()
{
    //FragColor = colMult * 2;
    //FragColor = vec4(outColor * ourColor.rgb, 1.0);
    //FragColor = mix(texture(texture1, UV), texture(texture2, UV), sin(time + id) * 0.5 + 0.5) * colMult;
    //FragColor = vec4(Norm * 0.5f + 0.5f, 1.0f);
	//return ;
	//float ran1 = random(vec2(id, 1));
	//float ran2 = random(vec2(1.0f, ran1));
	//float ran3 = random(vec2(ran2, ran1));
	float norm = dot(normalize(Norm), vec3(0.0f, 0.25f, 0.75f));
	norm = clamp(norm, 0.0f, 1.0f);
    FragColor = texture(texture1, UV) * vec4(ranCol, 1.0f) * 2 * norm;
}