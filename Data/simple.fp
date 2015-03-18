uniform sampler2D texture;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].st); 
    gl_FragColor = vec4(color[0], color[1], color[2], color[3]);
	//vec4(0.5, 0.0, 0.0, 1.0);
}